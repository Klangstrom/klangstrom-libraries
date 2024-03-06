#ifdef __cplusplus
extern "C" {
#endif

#include "WM8904.h"
#include "KLST_PANDA-SerialDebug.h"

//#define WM8904_TIMEOUT     HAL_MAX_DELAY
#define WM8904_TIMEOUT     1000

static HAL_StatusTypeDef ret;
static I2C_HandleTypeDef *hi2c = 0;

uint8_t WM8904_write_register(uint8_t register_address, uint16_t data) {
    if (!hi2c) {
        println("WM8904 not initialized!");
        return HAL_ERROR;
    }
    uint8_t transmit_buffer[3];
    transmit_buffer[0] = register_address;
    transmit_buffer[1] = (data & 0xff00) >> 8;
    transmit_buffer[2] = data & 0xff;

    HAL_StatusTypeDef status = HAL_ERROR;
    uint8_t mAttempts = 8;
    while(status == HAL_ERROR && mAttempts > 0) {
        mAttempts--;
        status = HAL_I2C_Master_Transmit(hi2c, WM8904_I2C_ADDRESS, transmit_buffer, 3, WM8904_TIMEOUT);
        if (ret != HAL_OK) {
            println("WM8904: attempt: %i", mAttempts);
        }
    }
    if (status != HAL_OK) {
        println("WM8904: transmit I2C ERROR(W)");
        return HAL_ERROR;
    }
    return HAL_OK;
}

uint16_t WM8904_read_register(uint8_t register_address) {
    if (!hi2c) {
        println("WM8904 not initialized!");
        return HAL_ERROR;
    }
    uint8_t transmit_buffer[1];
    transmit_buffer[0] = register_address;
    ret = HAL_I2C_Master_Transmit(hi2c, WM8904_I2C_ADDRESS, transmit_buffer, 1, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        println("WM8904: transmit I2C ERROR(R0)");
        return HAL_ERROR;
    }

    uint8_t receive_buffer[2];
    ret = HAL_I2C_Master_Receive(hi2c, WM8904_I2C_ADDRESS, receive_buffer, 2, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        println("WM8904: receive I2C ERROR(R1)");
        return HAL_ERROR;
    }
    return (((uint16_t) receive_buffer[0] << 8) & 0xff00) | receive_buffer[1];
}

uint32_t WM8904_init(I2C_HandleTypeDef *hi2c_handle) {
    hi2c = hi2c_handle;

    if (!hi2c) {
        println("WM8904 not initialized!");
        return HAL_ERROR;
    }

    WM8904_write_register(WM8904_SW_RESET_AND_ID, 0xFFFF);
    uint16_t data = WM8904_read_register(WM8904_SW_RESET_AND_ID);
    if (data != 0x8904) {
        println("WM8904 not found!");
        return 1;
    } else {
        println("found WM8904");
    }
    return HAL_OK;
}

uint8_t WM8904_toggle_flag(uint8_t register_address, uint16_t flag_bit) {
    uint16_t register_value = WM8904_read_register(register_address);
    register_value ^= (1 << flag_bit);
    return WM8904_write_register(register_address, register_value);
}

uint8_t WM8904_set_flag(uint8_t register_address, uint16_t flag_bit, uint8_t flag_state) {
    uint16_t register_value = WM8904_read_register(register_address);
    if (flag_state) {
        register_value |= (1 << flag_bit);  // set to 1
    } else {
        register_value &= ~(1 << flag_bit); // set to 0
    }
    return WM8904_write_register(register_address, register_value);
}

#ifdef __cplusplus
}
#endif
