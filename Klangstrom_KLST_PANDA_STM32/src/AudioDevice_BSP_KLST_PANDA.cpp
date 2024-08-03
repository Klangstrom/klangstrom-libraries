/*
* Klangstrom
*
* This file is part of the *Klangstrom* library (https://github.com/dennisppaul/klangstrom-libraries).
* Copyright (c) 2024 Dennis P Paul.
*
* This library is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Klangstrom_ASP_KLST_STM32-Config.h" // TODO change this to KLST_STM32 aka 'Architecture Specific' (ASP)
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC
#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32


#ifdef __cplusplus
extern "C" {
#endif

#include <cstring>

#include "stm32h7xx_hal.h"

#include "AudioDevice.h"
#include "AudioDevice_ASP_STM32.h"
#include "Console.h"
#include "sai.h"
#include "i2c.h"
#include "WM8904.h"
#include "WM8904Configure.h"
#include "DMAMemoryAllocator.h"

#ifndef AUDIOCODEC_RX
#define AUDIOCODEC_RX hsai_BlockA1
#endif // AUDIOCODEC_RX
#ifndef AUDIOCODEC_TX
#define AUDIOCODEC_TX hsai_BlockB1
#endif // AUDIOCODEC_TX
#ifndef AUDIOCODEC_CONFIG
#define AUDIOCODEC_CONFIG hi2c4
#endif // AUDIOCODEC_CONFIG

extern SAI_HandleTypeDef AUDIOCODEC_RX;
extern SAI_HandleTypeDef AUDIOCODEC_TX;
extern I2C_HandleTypeDef AUDIOCODEC_CONFIG;

/* TODO --------------------------------------------------------------------- */
// currently the only possible values
#define M_BIT_DEPTH 16
#define M_SAMPLE_RATE 48000
#define M_OUTPUT_CHANNELS 2
#define M_INPUT_CHANNELS 2
#define M_SAMPLES_PER_AUDIO_BLOCK 128
#define M_I2S_BUFFER_SIZE (M_SAMPLES_PER_AUDIO_BLOCK * 2)

uint32_t __attribute__((section(".dma_buffer"))) dma_TX_buffer[M_I2S_BUFFER_SIZE];
uint32_t __attribute__((section(".dma_buffer"))) dma_RX_buffer[M_I2S_BUFFER_SIZE];
uint32_t*                                        mCurrentInputRXBuffer;
uint32_t*                                        mCurrentOutputTXBuffer;
/* TODO --------------------------------------------------------------------- */

static void process_audioblock_data(AudioDevice* audiodevice, uint32_t* input, uint32_t* output) {
    AudioBlock&    audioblock      = *audiodevice->audioblock;
    const uint32_t sample_rate     = audiodevice->audioinfo->sample_rate;
    const uint8_t  output_channels = audiodevice->audioinfo->output_channels;
    const uint8_t  input_channels  = audiodevice->audioinfo->input_channels;
    const uint16_t block_size      = audiodevice->audioinfo->block_size;
    const uint8_t  bit_depth       = audiodevice->audioinfo->bit_depth;

    /* unpack receive buffer to (float) samples */
    constexpr float    M_INT_SCALE  = 1 << 15;    // Assuming 16-bit depth for simplicity
    constexpr uint32_t M_MASK_LEFT  = 0xFFFF;     // Assuming 16-bit left
    constexpr uint32_t M_MASK_RIGHT = 0xFFFF0000; // Assuming 16-bit right
    for (uint16_t i = 0; i < block_size; i++) {
        const uint32_t p         = input[i];
        const int16_t  mLeftInt  = static_cast<int16_t>(p & M_MASK_LEFT);
        const int16_t  mRightInt = static_cast<int16_t>((p & M_MASK_RIGHT) >> 16);
        if (input_channels > 0) {
            audioblock.input[0][i] = static_cast<float>(mLeftInt) / M_INT_SCALE;
        }
        if (input_channels > 1) {
            audioblock.input[1][i] = static_cast<float>(mRightInt) / M_INT_SCALE;
        }
    }

    audioblock.sample_rate     = sample_rate;
    audioblock.output_channels = output_channels;
    audioblock.input_channels  = input_channels;
    audioblock.block_size      = block_size;
    audioblock.device_id       = audiodevice->audioinfo->device_id;

    if (audiodevice->callback_audioblock != nullptr) {
        audiodevice->callback_audioblock(audiodevice->audioblock);
    }

    /* pack samples for transmit buffer */
    for (uint16_t i = 0; i < block_size; i++) {
        int16_t mLeftInt  = static_cast<int16_t>(audioblock.output[0][i] * M_INT_SCALE);
        int16_t mRightInt = static_cast<int16_t>(audioblock.output[1][i] * M_INT_SCALE);
        output[i]         = static_cast<uint32_t>(static_cast<uint16_t>(mLeftInt)) | (static_cast<uint32_t>(static_cast<uint16_t>(mRightInt)) << 16);
    }
}

static void rx_input_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
        mCurrentInputRXBuffer = &(dma_RX_buffer[audiodevice->audioinfo->block_size]);
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
        mCurrentInputRXBuffer = &(dma_RX_buffer[0]);
    }
}

static void tx_output_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    if (callback_type == CALLBACK_FULL_COMPLETE) {
        mCurrentOutputTXBuffer = &(dma_TX_buffer[audiodevice->audioinfo->block_size]);
    } else if (callback_type == CALLBACK_HALF_COMPLETE) {
        mCurrentOutputTXBuffer = &(dma_TX_buffer[0]);
    }
    process_audioblock_data(audiodevice, mCurrentInputRXBuffer, mCurrentOutputTXBuffer);
}

static void error_callback(AudioDevice* audiodevice, uint8_t callback_type) {
    (void) audiodevice;
    (void) callback_type;
}

static void delay_ms(uint32_t duration) {
    HAL_Delay(duration);
}

static void setup_SAI(AudioDevice* audiodevice) {
    console_status("setting up SAI");

    memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
    memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));

    HAL_StatusTypeDef status;
    //    status = HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*) dma_TX_buffer, M_I2S_BUFFER_SIZE * 2); // TODO a/ why is this `*2` necessary? b/ replace with dynamic size
    status = HAL_SAI_Transmit_DMA(&AUDIOCODEC_TX, (uint8_t*) dma_TX_buffer, M_I2S_BUFFER_SIZE * 2);
    if (HAL_OK != status) {
        console_error("### ERROR initializing SAI TX: %i", status);
    }

    //    status = HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*) dma_RX_buffer, M_I2S_BUFFER_SIZE * 2); // TODO a/ why is this `*2` necessary? b/ replace with dynamic size
    status = HAL_SAI_Receive_DMA(&AUDIOCODEC_RX, (uint8_t*) dma_RX_buffer, M_I2S_BUFFER_SIZE * 2);
    if (HAL_OK != status) {
        console_error("### ERROR initializing SAI RX: %i", status);
    }

    mCurrentInputRXBuffer  = &(dma_RX_buffer[0]);
    mCurrentOutputTXBuffer = &(dma_TX_buffer[0]);
}

static void check_audiodevice_config(AudioDevice* audiodevice) {
    if (audiodevice->audioinfo->sample_rate != M_SAMPLE_RATE) {
        console_error("### ERROR @check_audiodevice_config / sample rate not supported\n");
        return;
    }
    if (audiodevice->audioinfo->output_channels != M_OUTPUT_CHANNELS) {
        console_error("### ERROR @check_audiodevice_config / output channels not supported\n");
        return;
    }
    if (audiodevice->audioinfo->input_channels != M_INPUT_CHANNELS) {
        console_error("### ERROR @check_audiodevice_config / input channels not supported\n");
        return;
    }
    if (audiodevice->audioinfo->block_size != M_SAMPLES_PER_AUDIO_BLOCK) {
        console_error("### ERROR @check_audiodevice_config / block size not supported\n");
        return;
    }
    if (audiodevice->audioinfo->bit_depth != M_BIT_DEPTH) {
        console_error("### ERROR @check_audiodevice_config / bit depth not supported\n");
        return;
    }
}

void audiodevice_init_device_BSP(AudioDevice* audiodevice) {
    audiodevice->peripherals->callback_rx    = rx_input_callback;
    audiodevice->peripherals->callback_tx    = tx_output_callback;
    audiodevice->peripherals->callback_error = error_callback;

    check_audiodevice_config(audiodevice);

    //    dma_TX_buffer = (uint32_t*) dma_malloc(M_I2S_BUFFER_SIZE * sizeof(uint32_t));
    //    dma_RX_buffer = (uint32_t*) dma_malloc(M_I2S_BUFFER_SIZE * sizeof(uint32_t));
    //    dma_free_all();

    MX_I2C4_Init();
    MX_SAI1_Init();
    if (WM8904_init(&AUDIOCODEC_CONFIG) == HAL_OK) {
    } else {
        console_error("### ERROR @audiodevice_init_device_BSP / error initializing audio codec\n");
    }
    delay_ms(100);
    WM8904_configure(true, false); // TODO see why `setup_default_start_sequence` needs to be called again
    setup_SAI(audiodevice);
    // TODO maybe pause audio device here
    //     audiodevice_pause(audiodevice);
}

void audiodevice_init_peripherals(AudioDevice* audiodevice) {
    audiodevice->peripherals                     = new AudioDevicePeripherals();
    audiodevice->peripherals->audiodevice_sai_rx = &AUDIOCODEC_RX;
    audiodevice->peripherals->audiodevice_sai_tx = &AUDIOCODEC_TX;
    audiodevice->peripherals->audiodevice_config = &AUDIOCODEC_CONFIG;
}

void audiodevice_deinit_BSP(AudioDevice* audiodevice) {
    (void) audiodevice; // TODO implement
}

void audiodevice_resume(AudioDevice* audiodevice) {
    HAL_SAI_DMAResume(audiodevice->peripherals->audiodevice_sai_tx);
    HAL_SAI_DMAResume(audiodevice->peripherals->audiodevice_sai_rx);
}

void audiodevice_pause(AudioDevice* audiodevice) {
    HAL_SAI_DMAPause(audiodevice->peripherals->audiodevice_sai_tx);
    HAL_SAI_DMAPause(audiodevice->peripherals->audiodevice_sai_rx);
}

#ifdef __cplusplus
}
#endif

#endif // KLST_PANDA_STM32
#endif // KLST_PANDA_ENABLE_AUDIOCODEC
