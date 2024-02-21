#include "stdio.h"
#include "main.h"
#include "KLST_PANDA-IDC_Serial.h"
#include "KLST_PANDA-SerialDebug.h"

#define BUFFER_SIZE 1

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart8;
extern UART_HandleTypeDef huart9;

uint8_t rxBuffer_00[BUFFER_SIZE];
uint8_t rxBuffer_01[BUFFER_SIZE];
uint8_t rxBuffer_MIDI[BUFFER_SIZE];

volatile bool receive_00 = false;
volatile bool receive_01 = false;
volatile bool receive_MIDI = false;

static void print_and_clear_buffer(const char *name, UART_HandleTypeDef *huart,
        uint8_t *buffer, uint8_t buffer_size) {
    printf("%s (", name);
    for (int i = 0; i < buffer_size; i++) {
        printf("0x%X, ", buffer[i]);
        buffer[i] = 0;
    }
    printf(")\r\n");
}

static void evaluate_receive_flags() {
    if (receive_00 || receive_01 || receive_MIDI) {
        print("data_receive : ");
    }
    if (receive_00) {
        print_and_clear_buffer("UART9", &huart9, rxBuffer_00, BUFFER_SIZE);
        receive_00 = false;
    } else if (receive_01) {
        print_and_clear_buffer("UART8", &huart8, rxBuffer_01, BUFFER_SIZE);
        receive_01 = false;
    } else if (receive_MIDI) {
        print_and_clear_buffer("UART4", &huart4, rxBuffer_MIDI, BUFFER_SIZE);
        receive_MIDI = false;
    }
}

void IDC_serial_setup() {
    // 64000000Hz/(31250Baud*16) = 128

    // TODO MIDI ( move to own file )
    // UART4 > _MIDI_ANALOG_IN + _MIDI_ANALOG_OUT
    IDC_serial_start_rx_interrupt(UART4);
    IDC_serial_start_rx_interrupt(UART8);
    IDC_serial_start_rx_interrupt(UART9);
//    HAL_UART_Receive_IT(&huart4, rxBuffer_MIDI, BUFFER_SIZE);
//    HAL_UART_Receive_IT(&huart9, rxBuffer_00, BUFFER_SIZE);
//    HAL_UART_Receive_IT(&huart8, rxBuffer_01, BUFFER_SIZE);
}

void IDC_serial_loop() {
    evaluate_receive_flags();

    //    println("transmit uart4");
    uint8_t data[3] = { 0xF2, 0x20, 0x02 };
    //    HAL_UART_Transmit_IT(&huart4, (uint8_t*) data, BUFFER_SIZE);
    println("data_transmit: UART9 + UART8 + UART4");
    HAL_UART_Transmit_IT(&huart9, (uint8_t*) data, BUFFER_SIZE);
    HAL_UART_Transmit_IT(&huart8, (uint8_t*) data, BUFFER_SIZE);
    HAL_UART_Transmit_IT(&huart4, (uint8_t*) data, BUFFER_SIZE);
    //    HAL_UART_Transmit_IT(&huart9, (uint8_t*) data, 3);

    //  print("UART9: ");
    //  HAL_UART_Receive(&huart9, rxBuffer_00, sizeof(rxBuffer_00), 1000);
    //  for (int i = 0; i < (uint8_t) sizeof(rxBuffer_00); i++) {
    //      printf("0x%X, ", rxBuffer_00[i]);
    //      rxBuffer_00[i] = 0;
    //  }
    //  printf("\r\n");
    //
    //  HAL_UART_Receive(&huart4, rxBuffer_MIDI, BUFFER_SIZE, 1000);
    //  print("UART4: ");
    //  for (int i = 0; i < 3; i++) {
    //      printf("0x%X, ", rxBuffer_MIDI[i]);
    //      rxBuffer_MIDI[i] = 0;
    //  }
    //  printf("\r\n");
}

void IDC_serial_start_rx_interrupt(USART_TypeDef *uart_instance) {
    if (uart_instance == UART9) {
        HAL_UART_Receive_IT(&huart9, rxBuffer_00, BUFFER_SIZE);
    } else if (uart_instance == UART8) {
        HAL_UART_Receive_IT(&huart8, rxBuffer_01, BUFFER_SIZE);
    } else if (uart_instance == UART4) {
        HAL_UART_Receive_IT(&huart4, rxBuffer_MIDI, BUFFER_SIZE);
    }
}
