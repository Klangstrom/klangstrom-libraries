#include "stdio.h"
#include "stdbool.h"
#include "main.h"
#include "KLST_PANDA-IDC_Serial.h"
#include "KLST_PANDA-SerialDebug.h"

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;

extern DMA_HandleTypeDef hdma_uart8_rx;

uint8_t RX_00_buffer[BUFFER_SIZE];
//uint8_t RX_01_buffer[BUFFER_SIZE];
uint8_t RX_MIDI_buffer[BUFFER_SIZE];
uint8_t __attribute__((section(".dma_buffer"))) RX_01_DMA_buffer[DMA_BUFFER_SIZE];
uint8_t __attribute__((section(".dma_buffer"))) TX_01_DMA_buffer[DMA_BUFFER_SIZE];

volatile uint8_t RX_00_counter;
//volatile uint8_t RX_01_counter;
volatile uint8_t RX_MIDI_counter;

uint8_t UART9_00_buffer[1];
//uint8_t UART8_01_buffer[1];
uint8_t UART4_MIDI_buffer[1];

static void print_and_clear_buffer(const char *name, uint8_t *buffer, uint8_t buffer_size) {
    printf("%s (", name);
    for (int i = 0; i < buffer_size; i++) {
        printf("0x%X, ", buffer[i]);
        buffer[i] = 0;
    }
    printf(") ");
}

static void evaluate_receive_flags() {
    bool mReceivedData = RX_00_counter > 0
//            || RX_01_counter > 0
            || RX_MIDI_counter > 0;
    if (mReceivedData) {
        print("data_receive : (");
    }
    if (RX_00_counter > 0) {
        print_and_clear_buffer("UART9", RX_00_buffer, RX_00_counter);
        RX_00_counter = 0;
    }
//    if (RX_01_counter > 0) {
//        print_and_clear_buffer("UART8", RX_01_buffer, RX_01_counter);
//        RX_01_counter = 0;
//    }
    if (RX_MIDI_counter > 0) {
        print_and_clear_buffer("UART4", RX_MIDI_buffer, RX_MIDI_counter);
        RX_MIDI_counter = 0;
    }
    if (mReceivedData) {
        printf("\r\n");
    }
}

void IDC_serial_setup() {
    RX_00_counter = 0;
//    RX_01_counter = 0;
    RX_MIDI_counter = 0;
    // 64000000Hz/(31250Baud*16) = 128

    // TODO MIDI ( move to own file )
    // UART4 > _MIDI_ANALOG_IN + _MIDI_ANALOG_OUT
    IDC_serial_handle_rx(UART4);
//    IDC_serial_handle_rx(UART8);
    IDC_serial_handle_rx(UART9);

    HAL_UARTEx_ReceiveToIdle_DMA(&huart8, RX_01_DMA_buffer, DMA_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(&hdma_uart8_rx, DMA_IT_HT);

    println("UART: note UART8 is configured for DMA … but not handled TODO");
}

void IDC_serial_loop() {
    evaluate_receive_flags();

#define TX_BUFFER_SIZE 3
    uint8_t data[TX_BUFFER_SIZE];
//    println("data_transmit: UART9 + UART8 + UART4");
    println("data_transmit: UART9 + UART4");
    data[0] = 0xF2;
    data[1] = 0x20;
    data[2] = 0x01;
    HAL_UART_Transmit_IT(&huart9, (uint8_t*) data, TX_BUFFER_SIZE);
//    data[0] = 0xF3;
//    data[1] = 0x21;
//    data[2] = 0x02;
//    HAL_UART_Transmit_IT(&huart8, (uint8_t*) data, TX_BUFFER_SIZE);
    data[0] = 0xF4;
    data[1] = 0x22;
    data[2] = 0x03;
    HAL_UART_Transmit_IT(&huart4, (uint8_t*) data, TX_BUFFER_SIZE);

    data[0] = 0xF3;
    data[1] = 0x21;
    data[2] = 0x02;
    for (uint8_t i = 0; i < TX_BUFFER_SIZE; i++) {
        TX_01_DMA_buffer[i] = data[i];
    }
    HAL_UART_Transmit_DMA(&huart8, TX_01_DMA_buffer, TX_BUFFER_SIZE);
}

uint8_t IDC_serial_handle_rx(USART_TypeDef *uart_instance) {
    uint8_t mValue = 0;
    if (uart_instance == UART9) {
        mValue = UART9_00_buffer[0];
        HAL_UART_Receive_IT(&huart9, UART9_00_buffer, 1);
//    } else if (uart_instance == UART8) {
//        mValue = UART8_01_buffer[0];
//        HAL_UART_Receive_IT(&huart8, UART8_01_buffer, 1);
    } else if (uart_instance == UART4) {
        mValue = UART4_MIDI_buffer[0];
        HAL_UART_Receive_IT(&huart4, UART4_MIDI_buffer, 1);
    }
    return mValue;
}

// TODO move to KLST_PANDA.c

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    // check if the callback is called by UART8 peripheral
    if (huart->Instance == UART8) {
        print("data_receive : (");
        print_and_clear_buffer("UART8[DMA]", RX_01_DMA_buffer, Size);
        printf("\r\n");

        HAL_UARTEx_ReceiveToIdle_DMA(&huart8, RX_01_DMA_buffer, DMA_BUFFER_SIZE);
        __HAL_DMA_DISABLE_IT(&hdma_uart8_rx, DMA_IT_HT);
    }
}
