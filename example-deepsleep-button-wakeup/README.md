# Deep Sleep Button Wakeup - ESP32S3

This example demonstrates how to use deep sleep mode on the ESP32S3 and wake up using GPIO button presses. The device enters deep sleep to save power and wakes up when any of the configured buttons are pressed.

## Description

The ESP32S3 enters deep sleep mode and can be woken up by:
- **Button press on GPIO 2, 3, or 5** (any of them)
- **Timer** (60 seconds as backup)

The device tracks boot count using RTC memory, which persists through deep sleep cycles.

## Hardware

- **Board**: Seeed Studio XIAO ESP32S3
- **Wake-up Buttons**: GPIO 2, 3, 5
- **Connection**: Connect buttons between GPIO pins and GND (internal pull-ups are enabled)

## Features

- ✅ Multiple GPIO wake-up sources (GPIO 2, 3, 5)
- ✅ Timer-based wake-up (60 seconds backup)
- ✅ Boot counter using RTC memory
- ✅ Wake-up reason detection and reporting
- ✅ Real-time button state monitoring
- ✅ USB CDC serial output

## Deep Sleep Current Consumption

When in deep sleep mode:
- **Deep sleep**: ~10-150 µA (depending on configuration)
- **Active mode**: ~40-240 mA

This can significantly extend battery life for IoT applications.

## Usage

1. Build and upload:
   ```bash
   pio run -t upload
   ```

2. Open serial monitor:
   ```bash
   pio device monitor
   ```

3. The device will:
   - Print boot information
   - Show which button woke it up (if applicable)
   - Display current button states
   - Count down 5 seconds
   - Enter deep sleep

4. Press any button on GPIO 2, 3, or 5 to wake it up

## Output Example

```
=================================
Boot #1
=================================
⚡ First boot / Reset

📊 Current button states:
   GPIO 2: HIGH (not pressed)
   GPIO 3: HIGH (not pressed)
   GPIO 5: HIGH (not pressed)

📋 Configuring deep sleep...
✓ Wake-up enabled on GPIOs: 2, 3, 5 (press button to wake)
✓ Timer wake-up enabled: 60 seconds

⏱️  Device will sleep in 5 seconds...
   5...
   4...
   3...
   2...
   1...
💤 Going to deep sleep now...
   Press button on GPIO 2, 3, or 5 to wake up!
=================================

=================================
Boot #2
=================================
🟢 Wakeup caused by button press!
   → Button on GPIO 5 was pressed
```

## Technical Notes

- Uses `esp_sleep_enable_ext1_wakeup()` for multi-GPIO wake-up
- Wake-up triggers on LOW level (button pressed to GND)
- RTC_DATA_ATTR stores variables in RTC memory that survives deep sleep
- Internal pull-up resistors are enabled on all button GPIOs

## Power Saving Tips

- In real applications, disable USB CDC before sleep for lower power
- Consider using external pull-ups/pull-downs if needed
- Minimize peripheral initialization before sleep
- Use ULP coprocessor for even lower power operation

