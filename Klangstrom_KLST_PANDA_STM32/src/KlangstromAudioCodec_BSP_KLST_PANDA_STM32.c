/*
 * Klangstrom
 *
 * This file is part of the *wellen* library (https://github.com/dennisppaul/wellen).
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

#include "KlangstromEnvironment.h"
#ifdef KLST_PANDA_STM32
#include "Klangstrom_BSP_KLST_PANDA_STM32-Config.h"
#ifdef KLST_PANDA_ENABLE_AUDIOCODEC

#include "KlangstromDefines.h"

#if (KLANG_AUDIO_RATE!=48000) // TODO make the setup sequence aware of the current audio rate … currently fixed at 48KHz e.g `setup_SCLK_FLL()`
#warning "currently the audio codec only supports 48000Hz audio rate. KLANG_AUDIO_RATE is not set to 48000."
#endif

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stm32h7xx_hal.h"
#include "sai.h"

#include "KlangstromDefines.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromAudioCodec.h"
#include "KlangstromAudioCodec_BSP_KLST_PANDA_STM32.h"
#include "WM8904.h"

extern I2C_HandleTypeDef hi2c4;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;

#define I2S_BUFFER_SIZE (KLANG_SAMPLES_PER_AUDIO_BLOCK * 2)

uint32_t __attribute__((section(".dma_buffer"))) dma_TX_buffer[I2S_BUFFER_SIZE];
uint32_t __attribute__((section(".dma_buffer"))) dma_RX_buffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

static void delay_ms(uint32_t duration) {
    HAL_Delay(duration);
}

static void setup_SAI();
static void setup_FLL();
static void setup_WM8904(bool use_FLL, bool use_start_sequence);
static void setup_default_start_sequence();
static void setup_manually();

uint8_t KLST_BSP_audiocodec_init() {
    if (WM8904_init(&hi2c4) != HAL_OK) {
        KLST_BSP_serialdebug_println("could not initialize audiocodec");
        return HAL_ERROR;
    } else {
        MX_SAI1_Init();
        setup_SAI(); /* NOTE this is required to start the master clock */
        delay_ms(100);
        setup_WM8904(true, false);
        //           FLL   START_SEQUENCE
        setup_default_start_sequence(); // TODO investigate why this is necessary to generate decent volume ( albeit DC offset?!? )
        return HAL_OK;
    }
}

static void set_lineout_volume(uint8_t volume_left, uint8_t volume_right) {
    WM8904_write_register(WM8904_R59_ANALOGUE_OUT2_LEFT, WM8904_LINEOUT_VU | WM8904_LINEOUTL_VOL(volume_left));
    WM8904_write_register(WM8904_R60_ANALOGUE_OUT2_RIGHT, WM8904_LINEOUT_VU | WM8904_LINEOUTR_VOL(volume_right));
}

static void set_headphone_volume(uint8_t volume_left, uint8_t volume_right) {
    WM8904_write_register(WM8904_R57_ANALOGUE_OUT1_LEFT, WM8904_HPOUT_VU | WM8904_HPOUTL_VOL(volume_left));
    WM8904_write_register(WM8904_R58_ANALOGUE_OUT1_RIGHT, WM8904_HPOUT_VU | WM8904_HPOUTR_VOL(volume_right));
}

static void setup_manually() {
    /* --- POWER ---------------------------------------------------------------------------------------------------- */
    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS);
    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_FAST | WM8904_VMID_ENA);
    delay_ms(5);
    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_NORMAL | WM8904_VMID_ENA);
    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS | WM8904_BIAS_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
    WM8904_write_register(WM8904_DAC_DIGITAL_1, WM8904_DEEMPH(0));
    WM8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
    WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
    WM8904_write_register(WM8904_CLASS_W_0, WM8904_CP_DYN_PWR);

    /* --- AUDIO_INTERFACE ------------------------------------------------------------------------------------------ */

#ifdef TEST_WM8904_SET
    KLST_BSP_serialdebug_println("TEST register values below should not be the same");
    print("WM8904_R25_AUDIO_INTERFACE_1: ");
    console_binary(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1), 16); // TEST
#endif

    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, WM8904_AIF_WL_16BIT | WM8904_AIF_FMT_I2S);
    WM8904_write_register(WM8904_R26_AUDIO_INTERFACE_2, 0);
    WM8904_write_register(WM8904_R27_AUDIO_INTERFACE_3, 0);
    WM8904_write_register(WM8904_R18_WM8904_POWER_MANAGEMENT_6,
    WM8904_DACL_ENA |
    WM8904_DACR_ENA |
    WM8904_ADCL_ENA |
    WM8904_ADCR_ENA);
    delay_ms(5);

    /* --- INPUT_OUTPUT --------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, WM8904_LIN_VOL(0x10));
    WM8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, WM8904_RIN_VOL(0x10));
    WM8904_write_register(WM8904_R67_DC_SERVO_0,
    WM8904_DCS_ENA_CHAN_3 |
    WM8904_DCS_ENA_CHAN_2 |
    WM8904_DCS_ENA_CHAN_1 |
    WM8904_DCS_ENA_CHAN_0);
    WM8904_write_register(WM8904_R68_DC_SERVO_1,
    WM8904_DCS_TRIG_STARTUP_3 |
    WM8904_DCS_TRIG_STARTUP_2 |
    WM8904_DCS_TRIG_STARTUP_1 |
    WM8904_DCS_TRIG_STARTUP_0);
    delay_ms(100);

    /* --- LINE_OUT ------------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_R94_WM8904_ANALOGUE_LINEOUT_0,
    WM8904_LINEOUTL_ENA_OUTP |
    WM8904_LINEOUTL_ENA_DLY |
    WM8904_LINEOUTL_ENA |
    WM8904_LINEOUTR_ENA_OUTP |
    WM8904_LINEOUTR_ENA_DLY |
    WM8904_LINEOUTR_ENA);
    set_lineout_volume(0x32, 0x32); // TODO check if this is 80% ( with max 0x40 )
    delay_ms(100);

    /* --- HEADPHONES ----------------------------------------------------------------------------------------------- */

    WM8904_write_register(WM8904_R90_WM8904_ANALOGUE_HP_0,
    WM8904_HPL_ENA_OUTP |
    WM8904_HPL_ENA_DLY |
    WM8904_HPL_ENA |
    WM8904_HPR_ENA_OUTP |
    WM8904_HPR_ENA_DLY |
    WM8904_HPR_ENA);
    set_headphone_volume(0xFF, 0xFF);
    delay_ms(100);

#ifdef TEST_WM8904_SET
    print("WM8904_R25_AUDIO_INTERFACE_1: ");
    console_binary(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1), 16); // TEST
#endif
}

static void setup_default_start_sequence() {
    // SYSCLK required for write sequencer to run, SYSCLK_SRC = 0 (system clock comes from MCLK, not from FLL)
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);

    // use CONTROL WRITE SEQUENCER (p122ff)
    // when done writing it asserts the WSEQ_EINT flag in Register R127
    // R112 (70h) Write Sequencer 4, WSEQ_BUSY: 0 = Sequencer idle, 1 = Sequencer busy
    // "The Start-up sequence is initiated by writing 0100h to Register R111 (6Fh)"

    /* using the default startup sequence (p131) */
    //                                                          (R108 > 0100h) - This enables the Write Sequencer
    WM8904_write_register(WM8904_R108_WM8904_WRITE_SEQUENCER_0, WM8904_WSEQ_ENA);
    //                                                          (R111 > 0100h) - This starts the Write Sequencer at Index address 0 (00h)
    WM8904_write_register(WM8904_R111_WM8904_WRITE_SEQUENCER_3, WM8904_WSEQ_START);

#ifdef TEST_WM8904_SET
    KLST_BSP_serialdebug_println("starting WM8904_WSEQ_START");
    KLST_BSP_serialdebug_println("TEST register values below should not be the same");
    print("WM8904_R5_VMID_CONTROL_0: ");
    console_binary(WM8904_read_register(WM8904_R5_VMID_CONTROL_0), 16); // TEST
#endif
    /* "... this sequence takes approximately 300ms to run." ( at 12.288MHz ) */
    delay_ms(333);
#ifdef TEST_WM8904_SET
    print("WM8904_R5_VMID_CONTROL_0: ");
    console_binary(WM8904_read_register(WM8904_R5_VMID_CONTROL_0), 16); // TEST
    KLST_BSP_serialdebug_println("finished WM8904_WSEQ_START");
#endif
}

static void setup_SCLK_FLL() {
    //                                              select SYSCLK / fs ratio = 12288000Hz/48000Hz = 256 = 0b0011 = 3
    //                                              select Sample Rate (fs) = 44.1kHz, 48kHz = 0b101 = 5
    WM8904_write_register(WM8904_R21_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
    //                                              0 = SYSCLK = MCLK
    WM8904_write_register(WM8904_R20_CLOCK_RATES_0, 0x0000);
    //                                              SYSCLK Source Select to FLL output
    //                                              System Clock enable
    //                                              DSP Clock enable
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_SYSCLK_SRC | WM8904_CLK_SYS_ENA | WM8904_CLK_DSP_ENA);
}

static void setup_SCLK_MCLK() {
    //                                              select SYSCLK / fs ratio = 12288000Hz/48000Hz = 256 = 0b0011 = 3
    //                                              select Sample Rate (fs) = 44.1kHz, 48kHz = 0b101 = 5
    WM8904_write_register(WM8904_R21_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
    //                                              0 = SYSCLK = MCLK
    WM8904_write_register(WM8904_R20_CLOCK_RATES_0, 0x0000);
    //                                              DSP Clock enable
    //                                              System Clock enable
    //                                              ( SYSCLK Source is MCLK )
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_DSP_ENA | WM8904_CLK_SYS_ENA);
}

/**
 * see "FREQUENCY LOCKED LOOP (FLL)" (p104ff)
 *
 * The FLL operating frequency, FVCO is set according to the following equation:
 * FVCO = (FREF x N.K x FLL_FRATIO) ( "FVCO must be in the range 90-100 MHz" )
 *
 * FREF = 12500000Hz
 * FLL_FRATIO = 1MHz - 13.5MHz = 0h (divide by 1) = 0
 *
 * The FLL output frequency is generated according to the following equation:
 * FOUT = (FVCO / FLL_OUTDIV)
 * FLL_OUTDIV = 11.25 MHz - 12.5 MHz = 000111 (divide by 8) = 7
 *
 * FVCO = (FOUT x FLL_OUTDIV) = 12288000 * 8 = 98304000Hz
 * N.K  = FVCO / (FLL_FRATIO x FREF) = 98304000Hz / ( 1 * 12500000Hz ) = 7.86432
 * K    = 0.86432 * 65536 = 56644.07552 = 56644
 * N    = 7
 */
static void setup_FLL() {
    //                                               FLL Disable
    WM8904_write_register(WM8904_R116_FLL_CONTROL_1, 0x0000);
    //                                               FLL FOUT clock divider: 7=divide by 8
    //                                               FVCO clock divider:     0=divide by 1 TODO(not sure about this `4`)
    WM8904_write_register(WM8904_R117_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(0));
    //                                               Fractional multiply for FREF
    WM8904_write_register(WM8904_R118_FLL_CONTROL_3, WM8904_FLL_K(56644));
    //                                               Integer multiply for FREF
    WM8904_write_register(WM8904_R119_FLL_CONTROL_4, WM8904_FLL_N(7));
    //                                               FLL Clock source: 0 = MCLK
    //                                               Frequency of the FLL: 0 = FVCO / 1
    //                                               FLL Clock Reference Divider: 0 = MCLK / 1 "... bring the reference down to 13.5MHz or below."
    WM8904_write_register(WM8904_R120_FLL_CONTROL_5, WM8904_FLL_CLK_REF_SRC(0) | WM8904_FLL_CTRL_RATE(0) | WM8904_FLL_CLK_REF_DIV(0));
    //                                               FLL Fractional enable
    //                                               FLL Enable
    WM8904_write_register(WM8904_R116_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
    delay_ms(5);
}

static void setup_WM8904(bool use_FLL, bool use_start_sequence) {
    KLST_BSP_serialdebug_println("configuring WM8904 at I2C 0x%02X", WM8904_I2C_ADDRESS);
    if (use_FLL) {
        setup_FLL();
    }
    delay_ms(50);
    if (use_start_sequence) {
        setup_default_start_sequence();
    } else {
        setup_manually();
    }
    delay_ms(50);
    if (use_FLL) {
        setup_SCLK_FLL();
    } else {
        setup_SCLK_MCLK();
    }
    KLST_BSP_serialdebug_println("OK.");
}

/* ------------------------------------------------------------------------------------------------------------------ */

#define SANITY_TEST             0
#define SANITY_TEST_PASSTHROUGH 0
#define SANITY_TEST_NOISE       0
#define SANITY_TEST_MIC         0

#if SANITY_TEST

static const float M_MAX_FREQUENCEY = 440.0;
float mFreqL = M_MAX_FREQUENCEY * 0.25;
float mFreqR = M_MAX_FREQUENCEY * 0.5;
float left_osc_phi = 0;
float right_osc_phi = 0;
float left_osc_phi_inc = 0;
float right_osc_phi_inc = 0;

void FillBuffer(uint32_t *mTXBuffer, uint32_t *mRXBuffer, uint16_t len) {
    mFreqL++;
    mFreqR++;
    if (mFreqL > M_MAX_FREQUENCEY) {
        mFreqL = M_MAX_FREQUENCEY * 0.25;
    }
    if (mFreqR > M_MAX_FREQUENCEY) {
        mFreqR = M_MAX_FREQUENCEY * 0.25;
    }
    left_osc_phi_inc = mFreqL / (float) KLANG_AUDIO_RATE;
    right_osc_phi_inc = mFreqR / (float) KLANG_AUDIO_RATE;

    for (uint16_t i = 0; i < len; i++) {
#if SANITY_TEST_PASSTHROUGH
        mTXBuffer[i] = mRXBuffer[i];
#else
#if SANITY_TEST_NOISE
        mTXBuffer[i] = rand();
#else
#if SANITY_TEST_MIC
        uint16_t out = FifoRead();
        mTXBuffer[i] = ((uint32_t) out) << 0 | ((uint32_t) out) << 16;
#else
        const float mAmplitude = 1.0f;

        float mLeftf = (float) sin(left_osc_phi * 6.2832f) * mAmplitude;
        left_osc_phi += left_osc_phi_inc;
        left_osc_phi -= (float) ((uint16_t) left_osc_phi);
        int16_t mLefti = (int16_t) (mLeftf * 32767.0f);

        float mRightf = (float) sin(right_osc_phi * 6.2832f) * mAmplitude;
        right_osc_phi += right_osc_phi_inc;
        right_osc_phi -= (float) ((uint16_t) right_osc_phi);
        int16_t mRighti = (int16_t) (mRightf * 32767.0f);

        // both channels
        mTXBuffer[i] = ((uint32_t) (uint16_t) mLefti) << 0 | ((uint32_t) (uint16_t) mRighti) << 16;
#endif // SANITY_TEST_MIC
#endif // SANITY_TEST_NOISE
#endif // SANITY_TEST_PASSTHROUGH
    }
}
#endif

static void setup_SAI() {
    KLST_BSP_serialdebug_println("setting up SAI");

    srand(time(NULL));

    memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
    memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));

    HAL_StatusTypeDef status;
    status = HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*) dma_TX_buffer, I2S_BUFFER_SIZE << 1);
    if (HAL_OK != status) {
        KLST_BSP_serialdebug_println("### ERROR initializing SAI TX: %i", status);
    }

    status = HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*) dma_RX_buffer, I2S_BUFFER_SIZE << 1);
    if (HAL_OK != status) {
        KLST_BSP_serialdebug_println("### ERROR initializing SAI RX: %i", status);
    }

    mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

void KLST_PANDA_audiocodec_TX_full_complete_callback(SAI_HandleTypeDef *hsai) {
    if (hsai == &hsai_BlockB1) {
#if SANITY_TEST
        FillBuffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
        audiocodec_callback_class(mCurrentRXBuffer, &(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), I2S_BUFFER_SIZE >> 1);
#endif
    }
}

void KLST_PANDA_audiocodec_TX_half_complete_callback(SAI_HandleTypeDef *hsai) {
    if (hsai == &hsai_BlockB1) {
#if SANITY_TEST
        FillBuffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
        audiocodec_callback_class(mCurrentRXBuffer, &(dma_TX_buffer[0]), I2S_BUFFER_SIZE >> 1);
#endif
    }
}

void KLST_PANDA_audiocodec_RX_full_complete_callback(SAI_HandleTypeDef *hsai) {
    if (hsai == &hsai_BlockA1) {
        // TODO maybe better copy input buffer?
        mCurrentRXBuffer = &(dma_RX_buffer[I2S_BUFFER_SIZE >> 1]);
    }
}

void KLST_PANDA_audiocodec_RX_half_complete_callback(SAI_HandleTypeDef *hsai) {
    if (hsai == &hsai_BlockA1) {
        // TODO maybe better copy input buffer?
        mCurrentRXBuffer = &(dma_RX_buffer[0]);
    }
}

void KLST_PANDA_audiocodec_error_callback(SAI_HandleTypeDef *hsai) {
    if (hsai == &hsai_BlockA1) {
        KLST_BSP_serialdebug_println("### ERROR error in RX SAI:BlockA1");
    }
    if (hsai == &hsai_BlockB1) {
        KLST_BSP_serialdebug_println("### ERROR error in TX SAI:BlockB1");
    }
}

#endif // KLST_PANDA_ENABLE_AUDIOCODEC
#endif // KLST_PANDA_STM32
