#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);

static const uint32_t USB_DEVICE_UPDATE_INTERVAL = 1000;
uint32_t mTicks;

void USR_USB_Device_Init() {
	mTicks = HAL_GetTick();
	USBD_UsrLog("++ testing USBD_UsrLog\r\n");
}

void USR_USB_Device_Update() {
	if (HAL_GetTick() > mTicks + USB_DEVICE_UPDATE_INTERVAL) {
		HAL_GPIO_TogglePin(LED_01_GPIO_Port, LED_01_Pin);
		mTicks += USB_DEVICE_UPDATE_INTERVAL;

		typedef struct mouseHID_struct {
			uint8_t buttons;
			int8_t x;
			int8_t y;
			int8_t wheel;
		} mouseHID_t;
		mouseHID_t mouseHID;
		mouseHID.buttons = 0;
		mouseHID.x = 10;
		mouseHID.y = 0;
		mouseHID.wheel = 0;
		USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) (&mouseHID), sizeof(struct mouseHID_struct));
	}
}
