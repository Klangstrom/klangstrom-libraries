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

//static void configure_codec(void) {
//    uint16_t data = 0;
//    /* check that WM8904 is present */
//    WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
//    data = WM8904_read_register(WM8904_SW_RESET_AND_ID);
//    if (data != 0x8904) {
//        println("WM8904 not found!");
//    }
//
//    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS);
//    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_FAST | WM8904_VMID_ENA);
//    delay_ms(5);
//    WM8904_write_register(WM8904_VMID_CONTROL_0,
//    WM8904_VMID_BUF_ENA | WM8904_VMID_RES_NORMAL | WM8904_VMID_ENA);
//    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS | WM8904_BIAS_ENA);
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
//    WM8904_write_register(WM8904_DAC_DIGITAL_1, WM8904_DEEMPH(0));
//    WM8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
//    WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
////    WM8904_write_register(WM8904_CLASS_W_0, WM8904_CP_DYN_PWR);
//
////    WM8904_write_register(WM8904_DIGITAL_PULLS, WM8904_MCLK_PU | WM8904_DACDAT_PU | WM8904_LRCLK_PU | WM8904_BCLK_PU);
////    WM8904_write_register(WM8904_DIGITAL_PULLS, WM8904_MCLK_PD | WM8904_DACDAT_PD | WM8904_LRCLK_PD | WM8904_BCLK_PD);
//
////    WM8904_write_register(WM8904_FLL_CONTROL_1, 0x0000);
////    WM8904_write_register(WM8904_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(4));
////    WM8904_write_register(WM8904_FLL_CONTROL_3, WM8904_FLL_K(0x8000));
////    WM8904_write_register(WM8904_FLL_CONTROL_4, WM8904_FLL_N(0xBB));
////    WM8904_write_register(WM8904_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
////    delay_ms(5);
////    WM8904_write_register(WM8904_FLL_CONTROL_1, 0x0000);
////    WM8904_write_register(WM8904_FLL_CONTROL_5, WM8904_FLL_CLK_REF_DIV(0));
////    WM8904_write_register(WM8904_FLL_CONTROL_2, WM8904_FLL_OUTDIV(7) | WM8904_FLL_FRATIO(0) | WM8904_FLL_CTRL_RATE(0));
////    WM8904_write_register(WM8904_FLL_CONTROL_3, WM8904_FLL_K(0x8000));
////    WM8904_write_register(WM8904_FLL_CONTROL_4, WM8904_FLL_N(0xBB) | WM8904_FLL_GAIN(0));
////    WM8904_write_register(WM8904_FLL_CONTROL_1, WM8904_FLL_FRACN_ENA | WM8904_FLL_ENA);
////    delay_ms(5);
////    WM8904_write_register(WM8904_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
////    WM8904_write_register(WM8904_CLOCK_RATES_0, 0x0000);
////    WM8904_write_register(WM8904_CLOCK_RATES_2, WM8904_SYSCLK_SRC | WM8904_CLK_SYS_ENA | WM8904_CLK_DSP_ENA);
////    delay_ms(5);
////    WM8904_write_register(WM8904_CLOCK_RATES_0, 0);
////    WM8904_write_register(WM8904_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5)); // 12.5MHz / 48KHz = 260 (256 > 3 > 0b0011)
////    WM8904_write_register(WM8904_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
////    delay_ms(5);
////    WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_BCLK_DIR | WM8904_AIF_FMT_I2S);
////    WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8));
////    WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_DIR | WM8904_LRCLK_RATE(0x20));
////    WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
////            WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
////    delay_ms(5);
////    WM8904_write_register(WM8904_AUDIO_INTERFACE_0, WM8904_LOOPBACK);
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8)); // 12.5MHz / ( 16bit * 2ch * 48KHz ) = 8 (0b01000)
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_RATE(0x20));
//    delay_ms(5);
//
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
//    WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
//    delay_ms(5);
//
//    WM8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, WM8904_LIN_VOL(0x10));
//    WM8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, WM8904_RIN_VOL(0x10));
//    WM8904_write_register(WM8904_ANALOGUE_HP_0, WM8904_HPL_ENA | WM8904_HPR_ENA);
//    WM8904_write_register(WM8904_ANALOGUE_HP_0, WM8904_HPL_ENA_DLY | WM8904_HPL_ENA |
//    WM8904_HPR_ENA_DLY | WM8904_HPR_ENA);
//    WM8904_write_register(WM8904_DC_SERVO_0, WM8904_DCS_ENA_CHAN_3 | WM8904_DCS_ENA_CHAN_2 |
//    WM8904_DCS_ENA_CHAN_1 | WM8904_DCS_ENA_CHAN_0);
//    WM8904_write_register(WM8904_DC_SERVO_1, WM8904_DCS_TRIG_STARTUP_3 | WM8904_DCS_TRIG_STARTUP_2 |
//    WM8904_DCS_TRIG_STARTUP_1 | WM8904_DCS_TRIG_STARTUP_0);
//    delay_ms(100);
//    WM8904_write_register(
//    WM8904_ANALOGUE_HP_0,
//    WM8904_HPL_RMV_SHORT |
//    WM8904_HPL_ENA_OUTP |
//    WM8904_HPL_ENA_DLY |
//    WM8904_HPL_ENA |
//    WM8904_HPR_RMV_SHORT |
//    WM8904_HPR_ENA_OUTP |
//    WM8904_HPR_ENA_DLY |
//    WM8904_HPR_ENA);
//
//    WM8904_write_register(
//    WM8904_ANALOGUE_OUT1_LEFT,
//    WM8904_HPOUT_VU |
//            WM8904_HPOUTL_VOL(0x39));
//    WM8904_write_register(
//    WM8904_ANALOGUE_OUT1_RIGHT,
//    WM8904_HPOUT_VU |
//            WM8904_HPOUTR_VOL(0x39));
//
//    WM8904_write_register(
//    WM8904_DAC_DIGITAL_VOLUME_LEFT,
//    WM8904_DACL_VOL(0xFF) |
//    WM8904_DAC_VU);
//    WM8904_write_register(
//    WM8904_DAC_DIGITAL_VOLUME_RIGHT,
//    WM8904_DACR_VOL(0xFF) |
//    WM8904_DAC_VU);
//
//    WM8904_write_register(
//    WM8904_DAC_DIGITAL_1,
//    WM8904_DAC_MUTE);
//
//    WM8904_write_register(
//    WM8904_ANALOGUE_LINEOUT_0,
//    WM8904_LINEOUTL_ENA |
//    WM8904_LINEOUTR_ENA |
//    WM8904_LINEOUTL_ENA_OUTP |
//    WM8904_LINEOUTR_ENA_OUTP);
//    delay_ms(20);
//
//    WM8904_write_register(
//    WM8904_ANALOGUE_LINEOUT_0,
//    WM8904_LINEOUTL_ENA_DLY |
//    WM8904_LINEOUTR_ENA_DLY);
//
//    delay_ms(100);
//}

static void setup_SAI();

//static void test_register(uint8_t register_addr, uint8_t iterations) {
//    println("test_register: %i", register_addr);
//    for (uint8_t i = 0; i < iterations; ++i) {
//        print_binary16ui(WM8904_read_register(register_addr));
//        delay_ms(100);
//    }
//}

//static uint8_t read_register(uint8_t *value) {
//    static const uint8_t mLength = 1;
//    uint8_t buf[1];
//    HAL_StatusTypeDef ret = HAL_I2C_Master_Receive(&hi2c4, 0x34, buf, mLength, 100);
//
//    if (ret != HAL_OK) {
//        println("WM8904: receive I2C ERROR");
//        return HAL_ERROR;
//    }
//    *value = buf[0];
//    return HAL_OK;
//}

static void setup_clock() {
    println("WM8904_R116-120_FLL_CONTROL ( before )");
    print_binary16ui(WM8904_read_register(WM8904_R116_FLL_CONTROL_1));
    print_binary16ui(WM8904_read_register(WM8904_R117_FLL_CONTROL_2));
    print_binary16ui(WM8904_read_register(WM8904_R118_FLL_CONTROL_3));
    print_binary16ui(WM8904_read_register(WM8904_R119_FLL_CONTROL_4));
    print_binary16ui(WM8904_read_register(WM8904_R120_FLL_CONTROL_5));

    /* generate SYSCLK from Frequency Locked Loop (FLL) using MCLK as reference */
    /* see "CLOCKING AND SAMPLE RATES" (p98ff) */
//    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, 0); // turn off system clock
    WM8904_write_register(WM8904_R21_CLOCK_RATES_1,
    WM8904_CLK_SYS_RATE(3) | // select SYSCLK / fs ratio = 12288000Hz/48000Hz = 256 = 0b0011 = 3
            WM8904_SAMPLE_RATE(5) // select Sample Rate (fs) = 44.1kHz, 48kHz = 0b101 = 5
    );

    /*
     see "FREQUENCY LOCKED LOOP (FLL)" (p104ff)

     The FLL operating frequency, FVCO is set according to the following equation:
     FVCO = (FREF x N.K x FLL_FRATIO) ( "FVCO must be in the range 90-100 MHz" )

     FREF = 12500000Hz
     FLL_FRATIO = 1MHz - 13.5MHz = 0h (divide by 1) = 0

     The FLL output frequency is generated according to the following equation:
     FOUT = (FVCO / FLL_OUTDIV)
     FLL_OUTDIV = 11.25 MHz - 12.5 MHz = 000111 (divide by 8) = 7

     FVCO = (FOUT x FLL_OUTDIV) = 12288000 * 8 = 98304000Hz
     N.K  = FVCO / (FLL_FRATIO x FREF) = 98304000Hz / ( 1 * 12500000Hz ) = 7.86432
     K    = 0.86432 * 65536 = 56644.07552 = 56644
     N    = 7
     */
    WM8904_write_register(WM8904_R116_FLL_CONTROL_1,
    WM8904_FLL_FRACN_ENA | // FLL Fractional enable
            WM8904_FLL_ENA // FLL Enable
    );
    WM8904_write_register(WM8904_R117_FLL_CONTROL_2,
    WM8904_FLL_OUTDIV(7) | // FLL FOUT clock divider
            WM8904_FLL_FRATIO(0) // FVCO clock divider
    );
    WM8904_write_register(WM8904_R118_FLL_CONTROL_3,
            WM8904_FLL_K(56644)); // Fractional multiply for FREF
    WM8904_write_register(WM8904_R119_FLL_CONTROL_4, WM8904_FLL_N(7)); // Integer multiply for FREF
    WM8904_write_register(WM8904_R120_FLL_CONTROL_5,
    WM8904_FLL_CLK_REF_SRC(0) | // FLL Clock source, 00 = MCLK
            WM8904_FLL_CTRL_RATE(0) | // Frequency of the FLL, 000 = FVCO / 1
            WM8904_FLL_CLK_REF_DIV(0) // FLL Clock Reference Divider, 00 = MCLK / 1 "... bring the reference down to 13.5MHz or below."
    );

    WM8904_write_register(WM8904_R22_CLOCK_RATES_2,
    WM8904_SYSCLK_SRC | // SYSCLK Source Select to FLL output
            WM8904_CLK_DSP_ENA | // DSP Clock enable
            WM8904_CLK_SYS_ENA | // System Clock enable
            WM8904_TOCLK_ENA // Zero Cross timeout enable
    );
    delay_ms(30);

    println("WM8904_R116-120_FLL_CONTROL ( after )");
    print_binary16ui(WM8904_read_register(WM8904_R116_FLL_CONTROL_1));
    print_binary16ui(WM8904_read_register(WM8904_R117_FLL_CONTROL_2));
    print_binary16ui(WM8904_read_register(WM8904_R118_FLL_CONTROL_3));
    print_binary16ui(WM8904_read_register(WM8904_R119_FLL_CONTROL_4));
    print_binary16ui(WM8904_read_register(WM8904_R120_FLL_CONTROL_5));
    println("---");
//    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, 0); // TODO so here is the problem!!! we should be able to write to the registers with the clock on!
    WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA | WM8904_SYSCLK_SRC);
    println("WM8904_R25_AUDIO_INTERFACE_1");
    print_binary16ui(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1));
    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, 0);
    print_binary16ui(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1));
    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
    print_binary16ui(WM8904_read_register(WM8904_R25_AUDIO_INTERFACE_1));
    println("---");

}

static void setup_audio() {
//    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS);
//    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_FAST | WM8904_VMID_ENA);
//
//    delay_ms(5);
//
//    WM8904_write_register(WM8904_VMID_CONTROL_0, WM8904_VMID_BUF_ENA | WM8904_VMID_RES_NORMAL | WM8904_VMID_ENA);
//    WM8904_write_register(WM8904_BIAS_CONTROL_0, WM8904_ISEL_HP_BIAS | WM8904_BIAS_ENA);
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_0, WM8904_INL_ENA | WM8904_INR_ENA);
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_2, WM8904_HPL_PGA_ENA | WM8904_HPR_PGA_ENA);
//    WM8904_write_register(WM8904_DAC_DIGITAL_1, WM8904_DEEMPH(0));
//    WM8904_write_register(WM8904_ANALOGUE_OUT12_ZC, 0x0000);
//    WM8904_write_register(WM8904_CHARGE_PUMP_0, WM8904_CP_ENA);
//
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_2, WM8904_BCLK_DIV(8));
//    WM8904_write_register(WM8904_AUDIO_INTERFACE_3, WM8904_LRCLK_RATE(0x20));
//
//    delay_ms(5);
//
//    WM8904_write_register(WM8904_POWER_MANAGEMENT_6,
//    WM8904_DACL_ENA | WM8904_DACR_ENA | WM8904_ADCL_ENA | WM8904_ADCR_ENA);
//
//    delay_ms(5);
//
//    WM8904_write_register(WM8904_ANALOGUE_LEFT_INPUT_0, WM8904_LIN_VOL(0x10));
//    WM8904_write_register(WM8904_ANALOGUE_RIGHT_INPUT_0, WM8904_RIN_VOL(0x10));
//
//    WM8904_write_register(WM8904_DC_SERVO_0,
//    WM8904_DCS_ENA_CHAN_3 |
//    WM8904_DCS_ENA_CHAN_2 |
//    WM8904_DCS_ENA_CHAN_1 |
//    WM8904_DCS_ENA_CHAN_0);
//    WM8904_write_register(WM8904_DC_SERVO_1,
//    WM8904_DCS_TRIG_STARTUP_3 |
//    WM8904_DCS_TRIG_STARTUP_2 |
//    WM8904_DCS_TRIG_STARTUP_1 |
//    WM8904_DCS_TRIG_STARTUP_0);
//
//    delay_ms(100);
//
//    WM8904_write_register( WM8904_ANALOGUE_HP_0,
//    WM8904_HPL_RMV_SHORT |
//    WM8904_HPL_ENA_OUTP |
//    WM8904_HPL_ENA_DLY |
//    WM8904_HPL_ENA |
//    WM8904_HPR_RMV_SHORT |
//    WM8904_HPR_ENA_OUTP |
//    WM8904_HPR_ENA_DLY |
//    WM8904_HPR_ENA);
//
//    WM8904_write_register(WM8904_ANALOGUE_OUT1_LEFT, WM8904_HPOUT_VU | WM8904_HPOUTL_VOL(0x39));
//    WM8904_write_register(WM8904_ANALOGUE_OUT1_RIGHT, WM8904_HPOUT_VU | WM8904_HPOUTR_VOL(0x39));
//
//    WM8904_write_register(WM8904_DAC_DIGITAL_VOLUME_LEFT, WM8904_DACL_VOL(0xFF) | WM8904_DAC_VU);
//    WM8904_write_register(WM8904_DAC_DIGITAL_VOLUME_RIGHT, WM8904_DACR_VOL(0xFF) | WM8904_DAC_VU);
//
//    WM8904_write_register( WM8904_DAC_DIGITAL_1, WM8904_DAC_MUTE);
//
//    WM8904_write_register(WM8904_ANALOGUE_LINEOUT_0,
//    WM8904_LINEOUTL_ENA |
//    WM8904_LINEOUTR_ENA |
//    WM8904_LINEOUTL_ENA_OUTP |
//    WM8904_LINEOUTR_ENA_OUTP |
//    WM8904_LINEOUTL_ENA_DLY |
//    WM8904_LINEOUTR_ENA_DLY
//            );
//    WM8904_write_register(WM8904_R33_DAC_DIGITAL_1, 0x0000);
//    delay_ms(20);
    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
    WM8904_write_register(WM8904_R26_AUDIO_INTERFACE_2, 0);
    WM8904_write_register(WM8904_R27_AUDIO_INTERFACE_3, 0);
    WM8904_write_register(WM8904_R33_DAC_DIGITAL_1, 0x0000);
}

static void setup_audio_start_sequence() {
    print_binary16ui(WM8904_read_register(WM8904_R5_VMID_CONTROL_0));
    // use CONTROL WRITE SEQUENCER (p122ff)
    // when done writing it asserts the WSEQ_EINT flag in Register R127
    // R112 (70h) Write Sequencer 4, WSEQ_BUSY: 0 = Sequencer idle, 1 = Sequencer busy
    // "The Start-up sequence is initiated by writing 0100h to Register R111 (6Fh)"

    /* using the default startup sequence (p131) */

    WM8904_write_register(WM8904_R108_WM8904_WRITE_SEQUENCER_0, WM8904_WSEQ_ENA); // (R108 > 0100h) - This enables the Write Sequencer
    WM8904_write_register(WM8904_R111_WM8904_WRITE_SEQUENCER_3, WM8904_WSEQ_START); // (R111 > 0100h) - This starts the Write Sequencer at Index address 0 (00h)

    print_binary16ui(WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4));
    delay_ms(50);
    print_binary16ui(WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4));
    delay_ms(50);
    print_binary16ui(WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4));
    delay_ms(50);
    print_binary16ui(WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4));
    delay_ms(50);

//    while (WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4) == WM8904_WSEQ_BUSY) { // (R112 = 0)
//        println("writer busy");
//        delay_ms(50);
//    }
//    // "... this sequence takes approximately 300ms to run."

    WM8904_write_register(WM8904_R33_DAC_DIGITAL_1, 0x0000);              // (R33  > 0000h) - This un-mutes the DACs
    WM8904_write_register(WM8904_R25_AUDIO_INTERFACE_1, WM8904_AIF_FMT_I2S | WM8904_AIF_WL_16BIT);
    WM8904_write_register(WM8904_R26_AUDIO_INTERFACE_2, 0);
    WM8904_write_register(WM8904_R27_AUDIO_INTERFACE_3, 0);

    print_binary16ui(WM8904_read_register(WM8904_R5_VMID_CONTROL_0));
}

uint8_t audiocodec_setup() {
    if (WM8904_init(&hi2c4) != HAL_OK) {
        println("could not initialize audiocodec");
        return HAL_ERROR;
    } else {
        println("configuring audiocodec at I2C 0x%02X", WM8904_I2C_ADDRESS);
        println("setup clock");
        setup_clock();
//        setup_audio();
        println("setup audio");
        setup_audio_start_sequence();

//        println("--------------------------");
//        uint8_t value = 0;
//        WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
//        while (read_register(&value) != HAL_ERROR) {
//            print_binary8ui(value);
//            delay_ms(100);
//        }
//        println("--------------------------");
//        for (uint8_t i = 0; i < 5; ++i) {
//            WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF); // reset
//            println("WM8904_SW_RESET_AND_ID: %02X", WM8904_read_register(WM8904_SW_RESET_AND_ID));
//            delay_ms(100);
//        }
//
//        println("WM8904_CLK_SYS_ENA");
//        WM8904_write_register(WM8904_R22_CLOCK_RATES_2, WM8904_CLK_SYS_ENA);
//        delay_ms(100);
//        test_register(WM8904_R18_WM8904_POWER_MANAGEMENT_6, 5);
//        test_register(WM8904_R33_DAC_DIGITAL_1, 5);
//        test_register(WM8904_CLOCK_RATES_0, 5);
//        test_register(WM8904_CLOCK_RATES_1, 5);
//        test_register(WM8904_CLOCK_RATES_1, 5);
//        test_register(WM8904_CLOCK_RATES_2, 5);
//
//        println("WM8904_CLK_SYS_ENA + WM8904_SYSCLK_SRC");
//        WM8904_write_register(WM8904_R20_CLOCK_RATES_0, 0);
//        WM8904_write_register(WM8904_R21_CLOCK_RATES_1, WM8904_CLK_SYS_RATE(3) | WM8904_SAMPLE_RATE(5));
//        WM8904_write_register(WM8904_R22_CLOCK_RATES_2,
//        WM8904_CLK_SYS_ENA | WM8904_SYSCLK_SRC | WM8904_OPCLK_ENA | WM8904_CLK_DSP_ENA | WM8904_TOCLK_ENA);
//        delay_ms(100);
//        test_register(WM8904_R18_WM8904_POWER_MANAGEMENT_6, 5);
//        test_register(WM8904_R33_DAC_DIGITAL_1, 5);
//        test_register(WM8904_CLOCK_RATES_0, 5);
//        test_register(WM8904_CLOCK_RATES_1, 5);
//        test_register(WM8904_CLOCK_RATES_2, 5);
//#define USE_STARTUP_SEQUENCE
//
//#ifndef USE_STARTUP_SEQUENCE
////        /* manual startup sequence */
////        WM8904_set_flag(WM8904_R18_WM8904_POWER_MANAGEMENT_6, WM8904_DACL_ENA_Pos, WM8904_ON); // enable DAC (p64)
////        WM8904_set_flag(WM8904_R18_WM8904_POWER_MANAGEMENT_6, WM8904_DACR_ENA_Pos, WM8904_ON);
//
//#else
//        /* (p70)
//         Under recommended usage conditions, all the control bits associated with enabling the Headphone
//         Outputs and the Line Outputs will be configured by scheduling the default Start-Up and Shutdown
//         sequences as described in the “Control Write Sequencer” section. In these cases, the user does not
//         need to set the register fields in R14, R15, R90 and R94 directly.
//         */
//
//        // use CONTROL WRITE SEQUENCER (p122ff)
//        // when done writing it asserts the WSEQ_EINT flag in Register R127
//        // R112 (70h) Write Sequencer 4, WSEQ_BUSY: 0 = Sequencer idle, 1 = Sequencer busy
//        // "The Start-up sequence is initiated by writing 0100h to Register R111 (6Fh)"
//        /* using the default startup sequence (p131) */
//        println("starting CONTROL WRITE SEQUENCER ...");
////        print_binary16ui(WM8904_read_register(WM8905_R4_BIAS_CONTROL_0));
////        print_binary16ui(WM8904_read_register(WM8904_R5_VMID_CONTROL_0));
////        print_binary16ui(WM8904_read_register(WM8904_R14_WM8904_POWER_MANAGEMENT_2));
////        print_binary16ui(WM8904_read_register(WM8904_R15_WM8904_POWER_MANAGEMENT_3));
////        print_binary16ui(WM8904_read_register(WM8904_R22_CLOCK_RATES_2));
//
//        WM8904_write_register(WM8904_R108_WM8904_WRITE_SEQUENCER_0, WM8904_WSEQ_ENA); // (R108 > 0100h) - This enables the Write Sequencer
//        WM8904_write_register(WM8904_R111_WM8904_WRITE_SEQUENCER_3, WM8904_WSEQ_START); // (R111 > 0100h) - This starts the Write Sequencer at Index address 0 (00h)
//        WM8904_write_register(WM8904_R33_DAC_DIGITAL_1, 0x0000);              // (R33  > 0000h) - This un-mutes the DACs
//        // ^^^ should this run before or after the loop?
//        while (WM8904_read_register(WM8904_R112_WM8904_WRITE_SEQUENCER_4) == WM8904_WSEQ_BUSY) { // (R112 = 0)
//            println("writer busy");
//            delay_ms(50);
//        }
//        // "... this sequence takes approximately 300ms to run."
////        println("---");
////        print_binary16ui(WM8904_read_register(WM8905_R4_BIAS_CONTROL_0));
////        print_binary16ui(WM8904_read_register(WM8904_R5_VMID_CONTROL_0));
////        print_binary16ui(WM8904_read_register(WM8904_R14_WM8904_POWER_MANAGEMENT_2));
////        print_binary16ui(WM8904_read_register(WM8904_R15_WM8904_POWER_MANAGEMENT_3));
////        print_binary16ui(WM8904_read_register(WM8904_R22_CLOCK_RATES_2));
//        println("... done");
//        delay_ms(400);
//
//#endif

        // DIGITAL AUDIO INTERFACE (p87)

        /* ----------------------------------------------------------------------------------------- */

        // note, at 12.5MHz clock speed duration of a cycle is: 80ns ( = 1000*1000*1000/12500000Hz )
//        WM8904_write_register(0x6C, 0x0100);
//        WM8904_write_register(0x6F, 0x0100);
//        WM8904_write_register(0x21, 0x0000);
//        configure_codec();
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
