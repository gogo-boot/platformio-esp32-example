# GPIO ID Check - Button Detector

This example helps you identify which GPIO pin is connected to a button on the Seeed XIAO ESP32S3.

## Description

Monitors all available GPIO pins on the XIAO ESP32S3 and reports when buttons are pressed or released. Perfect for identifying unknown GPIO connections.

## Hardware

- **Board**: Seeed Studio XIAO ESP32S3
- **Monitored GPIOs**: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 43, 44

## Usage

1. Build and upload:
   ```bash
   pio run -t upload
   ```

2. Open serial monitor:
   ```bash
   pio device monitor
   ```

3. Press any button connected to the board - the GPIO number will be displayed

## Output Example

```
=================================
GPIO Button Detector - XIAO ESP32S3
=================================
Press any button to see its GPIO number

Monitoring GPIOs:
  1 2 3 4 5 6 7 8 9 10 43 44 

🔴 BUTTON PRESSED on GPIO 5
🟢 BUTTON RELEASED on GPIO 5
```

## Notes

- All GPIOs are configured with internal pull-up resistors
- Buttons should connect GPIO to GND when pressed
- 50ms debounce delay is applied

