#include "KLST_PANDA-AudioCodec.h"
#include "KLST_PANDA-SerialDebug.h"
#include "main.h"
#include "WM8904.h"
#include <string.h>

extern I2C_HandleTypeDef hi2c4;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;

#define KLANG_AUDIO_RATE 48000
#define KLANG_SAMPLES_PER_AUDIO_BLOCK 256
#define I2S_BUFFER_SIZE (KLANG_SAMPLES_PER_AUDIO_BLOCK * 2)
//uint32_t dma_TX_buffer[I2S_BUFFER_SIZE] __attribute__((section(".dma_buffer")));
//uint32_t dma_RX_buffer[I2S_BUFFER_SIZE] __attribute__((section(".dma_buffer")));
uint32_t dma_TX_buffer[I2S_BUFFER_SIZE];
uint32_t dma_RX_buffer[I2S_BUFFER_SIZE];
uint32_t *mCurrentRXBuffer;

static void delay_ms(uint32_t duration) {
    HAL_Delay(duration);
}

static void configure_codec(void) {
    uint16_t data = 0;
    /* check that WM8904 is present */
    WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
    data = WM8904_read_register(WM8904_SW_RESET_AND_ID);
    if (data != 0x8904) {
        println("WM8904 not found!");
    }

    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS);
    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_FAST | WM8904_VMID_ENA);
    delay_ms(5);
    WM8904_write_register(WM8904_VMID_CONTROL_0,
    WM8904_VMID_BUF_ENA | WM8904_VMID_RES_NORMAL | WM8904_VMID_ENA);
    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS | WM8904_BIAS_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
    WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
    WM8904_write_register(WM8904_DAC_DIGITAL_1, WM8904_DEEMPH(0));
    WM8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
    WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
    WM8904_write_register(WM8904_CLASS_W_0, WM8904_CP_DYN_PWR);

//    WM8904_write_register(WM8904_FLL_CONTROL_1, 0x0000);
//    WM8904_write_register(WM8904_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(4));
//    WM8904_write_register(WM8904_FLL_CONTROL_3, WM8904_FLL_K(0x8000));
//    WM8904_write_register(WM8904_FLL_CONTROL_4, WM8904_FLL_N(0xBB));
//    WM8904_write_register(WM8904_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
//    delay_ms(5);
//    WM8904_write_register(WM8904_FLL_CONTROL_1, 0x0000);
//    WM8904_write_register(WM8904_FLL_CONTROL_5, WM8904_FLL_CLK_REF_DIV(0));
//    WM8904_write_register(WM8904_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(0) | WM8904_FLL_CTRL_RATE(0));
//    WM8904_write_register(WM8904_FLL_CONTROL_3, WM8904_FLL_K(0x8000));
//    WM8904_write_register(WM8904_FLL_CONTROL_4, WM8904_FLL_N(0xBB) | WM8904_FLL_GAIN(0));
//    WM8904_write_register(WM8904_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
//    delay_ms(5);
//    WM8904_write_register(WM8904_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
//    WM8904_write_register(WM8904_CLOCK_RATES_0, 0x0000);
//    WM8904_write_register(WM8904_CLOCK_RATES_2, WM8904_SYSCLK_SRC | WM8904_CLK_SYS_ENA | WM8904_CLK_DSP_ENA);
//    delay_ms(5);
    WM8904_write_register(WM8904_CLOCK_RATES_2, 0);
    WM8904_write_register(WM8904_CLOCK_RATES_0, 0);
    WM8904_write_register(WM8904_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5)); // 12.5MHz / 48KHz = 260 (256 > 3 > 0b0011)
    WM8904_write_register(WM8904_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
    delay_ms(5);
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_BCLK_DIR | WM8904_AIF_FMT_I2S);
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8));
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_DIR | WM8904_LRCLK_RATE(0x20));
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
//            WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
//    delay_ms(5);
    WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
    WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8)); // 12.5MHz / ( 16bit * 2ch * 48KHz ) = 8 (0b01000)
    WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_RATE(0x20));
    delay_ms(5);

    WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
    WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
    delay_ms(5);

    WM8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, WM8904_LIN_VOL(0x10));
    WM8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, WM8904_RIN_VOL(0x10));
    WM8904_write_register(WM8904_ANALOGUE_HP_0, WM8904_HPL_ENA | WM8904_HPR_ENA);
    WM8904_write_register(WM8904_ANALOGUE_HP_0, WM8904_HPL_ENA_DLY | WM8904_HPL_ENA |
    WM8904_HPR_ENA_DLY | WM8904_HPR_ENA);
    WM8904_write_register(WM8904_DC_SERVO_0, WM8904_DCS_ENA_CHAN_3 | WM8904_DCS_ENA_CHAN_2 |
    WM8904_DCS_ENA_CHAN_1 | WM8904_DCS_ENA_CHAN_0);
    WM8904_write_register(WM8904_DC_SERVO_1, WM8904_DCS_TRIG_STARTUP_3 | WM8904_DCS_TRIG_STARTUP_2 |
    WM8904_DCS_TRIG_STARTUP_1 | WM8904_DCS_TRIG_STARTUP_0);
    delay_ms(100);
    WM8904_write_register(WM8904_ANALOGUE_HP_0, WM8904_HPL_ENA_OUTP | WM8904_HPL_ENA_DLY | WM8904_HPL_ENA |
    WM8904_HPR_ENA_OUTP | WM8904_HPR_ENA_DLY | WM8904_HPR_ENA);
    WM8904_write_register(WM8904_ANALOGUE_HP_0,
    WM8904_HPL_RMV_SHORT | WM8904_HPL_ENA_OUTP | WM8904_HPL_ENA_DLY | WM8904_HPL_ENA |
    WM8904_HPR_RMV_SHORT | WM8904_HPR_ENA_OUTP | WM8904_HPR_ENA_DLY | WM8904_HPR_ENA);
    WM8904_write_register(WM8904_ANALOGUE_OUT1_LEFT, WM8904_HPOUT_VU | WM8904_HPOUTL_VOL(0x39));
    WM8904_write_register(WM8904_ANALOGUE_OUT1_RIGHT, WM8904_HPOUT_VU | WM8904_HPOUTR_VOL(0x39));
    delay_ms(100);
}

static void setup_SAI();

uint8_t audiocodec_setup() {
    if (WM8904_init(&hi2c4) != HAL_OK) {
        println("could not initialize audiocodec");
        return HAL_ERROR;
    } else {
        println("configuring audiocodec");

//        WM8904_write_register(0x6C, 0x0100);
//        WM8904_write_register(0x6F, 0x0100);
//        WM8904_write_register(0x21, 0x0000);

        configure_codec();

//        WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
//        delay_ms(100);
//
//        WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
//        WM8904_write_register(WM8904_CLASS_W_0, WM8904_CP_DYN_PWR);
//
//        WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
//        WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8)); // 12.5MHz / ( 16bit * 2ch * 48KHz ) = 8 (0b01000)
//        WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_RATE(0x20));
//        delay_ms(5);
//        WM8904_write_register(WM8904_CLOCK_RATES_0, 0);
//        WM8904_write_register(WM8904_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5)); // 12.5MHz / 48KHz = 260 (256 > 3 > 0b0011)
//        WM8904_write_register(WM8904_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
//        delay_ms(5);
//        WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
//        WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
//        WM8904_write_register(WM8904_POWER_MANAGEMENT_3, WM8904_LINEOUTL_PGA_ENA | WM8904_LINEOUTR_PGA_ENA);
//        WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
//        WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
//        delay_ms(100);
    }

    setup_SAI();

    return HAL_OK;
}

/* ------------------------------------------------------------------------------------------------------------------ */

#define SANITY_TEST             1
#define SANITY_TEST_PASSTHROUGH 0

#if SANITY_TEST

float left_osc_phi = 0;
float left_osc_phi_inc = 220.0f / (float) KLANG_AUDIO_RATE;  // generating 220Hz
float right_osc_phi = 0;
float right_osc_phi_inc = 110.0f / (float) KLANG_AUDIO_RATE;  // generating 110Hz

void FillBuffer(uint32_t *mTXBuffer, uint32_t *mRXBuffer, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
#if SANITY_TEST_PASSTHROUGH
        mTXBuffer[i] = mRXBuffer[i];
#else
        const float mAmplitude = 0.1f;

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
#endif
    }
}
#endif

static void setup_SAI() {
    memset(dma_TX_buffer, 0, sizeof(dma_TX_buffer));
    memset(dma_RX_buffer, 0, sizeof(dma_RX_buffer));

    HAL_StatusTypeDef status = HAL_SAI_Transmit_DMA(&hsai_BlockB1, (uint8_t*) dma_TX_buffer, I2S_BUFFER_SIZE << 1);
    if (HAL_OK != status) {
        println("### ERROR initializing SAI TX: %i", status);
    }

    status = HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*) dma_RX_buffer, I2S_BUFFER_SIZE << 1);
    if (HAL_OK != status) {
        println("### ERROR initializing SAI RX: %i", status);
    }

    mCurrentRXBuffer = &(dma_RX_buffer[0]);
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {
    printf("<");
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[I2S_BUFFER_SIZE >> 1]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
#if SANITY_TEST
    FillBuffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#else
    KLST_ISH_fill_buffer(&(dma_TX_buffer[0]), mCurrentRXBuffer, I2S_BUFFER_SIZE >> 1);
#endif
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
    printf(">");
    mCurrentRXBuffer = &(dma_RX_buffer[I2S_BUFFER_SIZE >> 1]);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {
    mCurrentRXBuffer = &(dma_RX_buffer[0]);
}
