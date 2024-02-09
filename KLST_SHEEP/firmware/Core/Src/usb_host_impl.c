#include "usbh_hid.h"

#define USR_USB_HOST_DEVICE_PASSTHROUGH

#ifdef USR_USB_HOST_DEVICE_PASSTHROUGH
#include "usb_device.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
extern uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);
#endif

void USR_USB_Host_Init() {
	USBH_UsrLog("++ testing USBH_UsrLog\r\n");
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
	if (USBH_HID_GetDeviceType(phost) == HID_MOUSE)  // if the HID is Mouse
			{
		HID_MOUSE_Info_TypeDef *Mouse_Info;
		Mouse_Info = USBH_HID_GetMouseInfo(phost);  // Get the info
		int X_Val = Mouse_Info->x;  // get the x value
		int Y_Val = Mouse_Info->y;  // get the y value
		if (X_Val > 127)
			X_Val -= 255;
		if (Y_Val > 127)
			Y_Val -= 255;
		printf("X=%d, Y=%d, Button1=%d, Button2=%d, Button3=%d\n\r", X_Val,
				Y_Val, Mouse_Info->buttons[0], Mouse_Info->buttons[1],
				Mouse_Info->buttons[2]);
//		int len = sprintf (Uart_Buf, "X=%d, Y=%d, Button1=%d, Button2=%d, Button3=%d\n", X_Val, Y_Val, Mouse_Info->buttons[0],Mouse_Info->buttons[1], Mouse_Info->buttons[2]);
//		HAL_UART_Transmit(&huart2, (uint8_t *) Uart_Buf, len, 100);

#ifdef USR_USB_HOST_DEVICE_PASSTHROUGH
		typedef struct mouseHID_struct {
			uint8_t buttons;
			int8_t x;
			int8_t y;
			int8_t wheel;
		} mouseHID_t;
		mouseHID_t mouseHID;
		mouseHID.buttons = Mouse_Info->buttons[0];
		mouseHID.x = X_Val;
		mouseHID.y = Y_Val;
		mouseHID.wheel = 0;
		USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) (&mouseHID),
				sizeof(struct mouseHID_struct));

#endif

	}

	if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD) // if the HID is Keyboard
			{
		uint8_t key;
		HID_KEYBD_Info_TypeDef *Keyboard_Info;
		Keyboard_Info = USBH_HID_GetKeybdInfo(phost);  // get the info
		key = USBH_HID_GetASCIICode(Keyboard_Info);  // get the key pressed
		printf("Key Pressed = %c\n\r", key);
//		int len = sprintf(Uart_Buf, "Key Pressed = %c\n", key);
//		HAL_UART_Transmit(&huart2, (uint8_t*) Uart_Buf, len, 100);
	}
}
