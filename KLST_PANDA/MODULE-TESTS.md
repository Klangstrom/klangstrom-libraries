# KLANGSTROM / KLST_PANDA / MODULE TESTS

## 1. Core Functionality Test

- [x] Power-up Test: Ensure the board powers up correctly.
- [x] Serial Debug: Verify that debugging information can be send via STD14 connector
- [x] Programmable LEDs: Verify that LEDs can be programmed
- [x] Clock Configuration: Verify the system clock settings.
	- [x] SYSCLK
	- [ ] @later(test peripheral clocks) @TODO
- [x] RAM Test: Check the integrity of the internal RAM.

## 2. Communication Interfaces Test

- [x] USB Device: Validate USB enumeration and data transfer.
- [x] USB Host: Test connection with external devices (keyboard, mouse).
- [ ] USART: Verify serial communication through 
	- [ ] USART2 and 
	- [x] USART3.
- [ ] SPI: Test SPI communication with a loopback or external device.
- [ ] I2C: Validate I2C communication with internal/external peripherals.

- Objective: Confirm that both USB host and device interfaces operate correctly.
- Steps for USB Device:
	- Configure the USB peripheral in device mode.
	- Connect the board to a PC and verify enumeration and communication.
- Steps for USB Host:
	- Configure the USB peripheral in host mode.
	- Connect a USB device (e.g., keyboard, mouse) and verify detection and basic functionality.

## 3. External Memory Test

- [ ] HYPERBUS™ Interface: Test communication with the S70KL1282 DRAM.

- Objective: Ensure proper communication and data integrity with the external DRAM.
- Steps:
	- Configure the memory controller of the STM32H723ZGT for the HYPERBUS™ interface.
	- Initialize the DRAM according to its initialization sequence.
	- Perform a basic read/write test by writing known data patterns to the DRAM and reading them back for verification.
	- Conduct a stress test by repeatedly writing and reading large data blocks.
	- Check for data integrity and any timing issues.

## 4. Display and Touch Interface Test

- [ ] LTDC: Validate basic color patterns and graphics on the TFT display.
- [ ] Touch Screen: Test the responsiveness and calibration of the touch interface.
- [ ] Backlight Control: Verify PWM control for dimming the backlight.
	- [x] works with GPIO
	- [ ] works with PWM

- Objective: Validate the functionality of the TFT display using the LTDC.
- Steps:
	- Initialize the LTDC peripheral with the correct resolution and pixel format settings.
	- Create a test pattern (e.g., color bars or gradients) and display it on the screen.
	- Test the screen refresh and response by updating the display with dynamic content.
	- Validate the touch functionality by capturing and processing touch inputs.
	- Check the backlight control through PWM dimming.

## 5. Audio Components Test

- [ ] Audio Codec: Test WM8904 functionality (playback and recording).
- [ ] Microphones: Validate audio capture from the onboard MEMS microphones.
- [ ] Headphone Output: Check the audio output through the headphone jack.

- Objective: Test the audio playback and recording capabilities of the WM8904 codec.
- Steps:
	- Initialize I2S and I2C peripherals for audio data and control interface.
	- Configure the WM8904 for standard playback and recording settings.
	- Play a pre-recorded audio file and verify the output on the headphone jack.
	- Record audio using the line-in or microphone input and store it in memory.
	- Play back the recorded audio to verify the recording path.

## 6. SD Card Interface Test

- [ ] SD Card Initialization: Confirm detection and initialization of the SD card.
- [ ] Read/Write Test: Perform read/write tests on the SD card.

- Objective: Confirm that the SD card interface works correctly for both reading and writing.
- Steps:
	- Initialize the SDIO/SDMMC interface with 4-bit wide communication settings.
	- Detect and identify the SD card using the standard initialization sequence.
	- Perform block read and write operations, checking for correct data transfer.
	- Execute a speed test by measuring the data transfer rate.
	- Optionally, test file system operations if a file system is implemented.

## 7. Analog and Digital I/O Test

- [ ] ADC: Test analog input readings.
- [ ] DAC: Validate analog output signals.
- [ ] GPIO: Check each GPIO pin for correct input/output functionality.

## 8. Rotary Encoders and Buttons Test

- [ ] Encoders: Test the functionality of the rotary encoders, including push buttons.
- [ ] Mechanical Buttons: Verify the operation of user-programmable and system buttons.

## 9. MIDI Interface Test

- [ ] MIDI-IN/OUT: Verify MIDI communication through UART and TRS connectors.

## 10. Power Management Test

- [ ] Battery Charging: Test the charging circuit and monitor battery voltage.
- [ ] Power Consumption: Measure power usage in various operation modes.

- Objective: Ensure the battery charges correctly and the power management functions as intended.
- Steps:
	- Monitor the battery voltage and charging current.
	- Validate the charging algorithm and the transition between charging states.
	- Test the power consumption in different operational modes (idle, active, sleep).
	- Verify the operation of power-related features like the on-off switch.

## 11. Additional Peripheral Tests

- [ ] LED Operation: Test the user-programmable LEDs for functionality and dimming.
- [ ] External Interrupts: Validate external interrupts (e.g., from touch screen or buttons).
- [ ] Timers: Check timers for accurate operation and interrupt generation.

## 12. Programmer and Debug Interface Test

- [ ] SWD Interface: Verify programming and debugging over SWD.
- [ ] Tag-Connect Interface: Test the alternative programming method.

## 13. Final Integration Test

- [ ] Run integrated tests involving multiple peripherals working simultaneously.
- [ ] Stress Test: Perform a system-level stress test to ensure stability under load.

## 14. Software and Firmware Test

- [ ] Firmware Functionality: Test all firmware features for correct operation.
- [ ] Error Handling: Verify system behavior under error conditions.
