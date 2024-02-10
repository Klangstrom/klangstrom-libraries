#ifdef __cplusplus
extern "C" {
#endif

#include "WM8904.h"
#include "KLST_PANDA-SerialDebug.h"

#define WM8904_TIMEOUT     HAL_MAX_DELAY

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
    ret = HAL_I2C_Master_Transmit(hi2c, WM8904_I2C_ADDRESS, transmit_buffer, 3, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        println("WM8904: transmit I2C ERROR");
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
        println("WM8904: transmit I2C ERROR");
        return HAL_ERROR;
    }

    uint8_t receive_buffer[2];
    ret = HAL_I2C_Master_Receive(hi2c, WM8904_I2C_ADDRESS, receive_buffer, 2, WM8904_TIMEOUT);
    if (ret != HAL_OK) {
        println("WM8904: receive I2C ERROR");
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
    uint16_t previous_register_value = register_value;
    if (flag_state) {
        register_value |= (1 << flag_bit);  // set to 1
    } else {
        register_value &= ~(1 << flag_bit); // set to 0
    }
//#define WM8904_SET_FLAG_TEST
#ifdef WM8904_SET_FLAG_TEST
    ret = WM8904_write_register(register_address, register_value);
    println("TOOD check if register was written: ");
    print("previous : ");
    print_binary16ui(previous_register_value);
    print("written  : ");
    print_binary16ui(register_value);
    print("read back: ");
    print_binary16ui(WM8904_read_register(register_address));
    return ret;
#else
    return WM8904_write_register(register_address, register_value);
#endif
}

#ifdef __cplusplus
}
#endif
