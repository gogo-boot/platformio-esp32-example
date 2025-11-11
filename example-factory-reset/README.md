# Factory Reset Example - ESP32 with Deep Sleep

This example demonstrates how to implement a factory reset function on ESP32 that erases all NVS (Non-Volatile Storage) data when a button is pressed and held for 5 seconds during wake-up. The device uses deep sleep between counter increments for ultra-low power consumption.

## Features

- 💤 Deep sleep mode (~10-150 µA current consumption)
- ⏰ Wakes every 10 seconds OR when button is pressed
- ✨ Auto-incrementing counter stored in NVS
- 📊 Counter value increments on each wake-up
- 💾 Persistent storage - counter survives reboots
- ⏱️ Long press detection (5 seconds) during wake-up
- 🗑️ Erase all NVS stored data with factory reset
- 🔄 Automatic restart after factory reset
- 📈 Counter resumes from last saved value after reboot
- 🔢 Counter resets to 0 after factory reset
- 🔋 Power efficient - sleeps between operations

## Hardware Requirements

- Seeed XIAO ESP32S3 (or any ESP32 board)
- Push button connected to GPIO 1 (with internal pull-up)

## Circuit

```
GPIO 1 ----[ Button ]---- GND
(Internal pull-up resistor enabled)
```

## How It Works

1. **Startup/Wake-up**: Device wakes from deep sleep (timer or button press)
2. **Wakeup Reason Check**: Displays why it woke up (button press, timer, or first boot)
3. **Factory Reset Check**: If button is pressed on wake-up:
   - Checks if button is still held
   - Waits for 5 seconds while monitoring button
   - If button held entire time → Factory reset and restart
   - If button released early → Continue normally
4. **Counter Increment**:
   - Loads counter from NVS
   - Increments by 1
   - Prints "Hello World [counter]" to serial
   - Saves to NVS
5. **Deep Sleep**: Device sleeps for 10 seconds (or until button press)
6. **Repeat**: Cycle continues, waking every 10 seconds or on button press

### Factory Reset Process
- Device is in deep sleep
- User presses button → Device wakes up
- **Keep holding button** for 5 seconds
- Device erases NVS and restarts
- Counter starts from 0 again

## Build and Upload

```bash
cd example-factory-reset
pio run --target upload
pio device monitor
```

## Expected Output

### First Boot (No NVS Data)
```
🏭 ================================
🏭 FACTORY RESET + DEEP SLEEP
🏭 ESP32 NVS Counter Demo
🏭 ================================

=================================
Boot #1
=================================
⚡ First boot / Reset

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 0
Free entries: 512
Total entries: 512
=================================

⚠️  No stored data found, starting from 0

📊 Hello World 1

📋 How it works:
=================================
Device sleeps for 10 seconds between counts.
Wake up by pressing button OR timer.
HOLD button for 5 seconds to factory reset!
=================================

⏱️  Device will sleep in 3 seconds...
   3...
   2...
   1...
💤 Going to deep sleep now...
   Will wake in 10 seconds or when button is pressed
   Hold button for 5 seconds during wake to factory reset!
=================================
```

### Timer Wake-up (Normal Operation)
```
🏭 ================================
🏭 FACTORY RESET + DEEP SLEEP
🏭 ESP32 NVS Counter Demo
🏭 ================================

=================================
Boot #2
=================================
⏰ Wakeup caused by timer

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 1
Free entries: 511
Total entries: 512
=================================

✅ Loaded counter from NVS: 1

📊 Hello World 2

📋 How it works:
=================================
Device sleeps for 10 seconds between counts.
Wake up by pressing button OR timer.
HOLD button for 5 seconds to factory reset!
=================================

⏱️  Device will sleep in 3 seconds...
   3...
   2...
   1...
💤 Going to deep sleep now...
```

### Button Wake-up (Quick Press - No Factory Reset)
```
🏭 ================================
🏭 FACTORY RESET + DEEP SLEEP
🏭 ESP32 NVS Counter Demo
🏭 ================================

=================================
Boot #5
=================================
🟢 Wakeup caused by button press!

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 1
Free entries: 511
Total entries: 512
=================================

🔵 Reset button detected on wake-up!
   Hold button for 5 seconds to factory reset...
🟢 Button released after 2.1 seconds
   (Not long enough for factory reset)

✅ Loaded counter from NVS: 4

📊 Hello World 5

💤 Going to deep sleep now...
```

### Factory Reset (Hold Button for 5 Seconds)
```
🏭 ================================
🏭 FACTORY RESET + DEEP SLEEP
🏭 ESP32 NVS Counter Demo
🏭 ================================

=================================
Boot #10
=================================
🟢 Wakeup caused by button press!

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 1
Free entries: 511
Total entries: 512
=================================

🔵 Reset button detected on wake-up!
   Hold button for 5 seconds to factory reset...
⏱️  Holding... 4 seconds remaining
⏱️  Holding... 3 seconds remaining
⏱️  Holding... 2 seconds remaining
⏱️  Holding... 1 seconds remaining
✅ Button held for 5 seconds!

🔥 ================================
🔥 FACTORY RESET INITIATED!
🔥 ================================

🗑️  Erasing NVS (Non-Volatile Storage)...
✅ NVS erased successfully!
✅ NVS reinitialized successfully!

✨ Factory reset complete!
   Counter will start from 0 again.

🔄 Restarting device in 3 seconds...
```

## Configuration

You can customize the following parameters in `main.cpp`:

```cpp
const gpio_num_t RESET_BUTTON_GPIO = GPIO_NUM_1;  // Reset button GPIO pin
const unsigned long HOLD_DURATION_MS = 5000;       // Hold time in milliseconds
const int SLEEP_DURATION_SEC = 10;                 // Deep sleep duration
const char* NVS_NAMESPACE = "storage";             // NVS namespace
const char* NVS_COUNTER_KEY = "counter";           // NVS key for counter
```

## Use Cases

- Learn NVS read/write operations with deep sleep
- Implement persistent data storage with low power consumption
- Factory reset for battery-powered production devices
- Clear all stored preferences and configuration data
- Testing NVS functionality with power management
- Understanding ESP32 non-volatile storage and deep sleep
- Ultra-low power counter applications
- IoT devices that wake periodically or on demand

## Notes

- The button uses internal pull-up resistor (active LOW)
- Button press wakes device from deep sleep
- **Important**: You must HOLD the button for the full 5 seconds after wake-up
- Counter increments only on wake-up (every 10 seconds or on button press)
- Counter value is saved to NVS on each wake-up
- Counter persists across reboots and deep sleep
- Device automatically restarts after factory reset
- All data in NVS partition will be permanently erased after reset
- Counter uses `int32_t` type (range: -2,147,483,648 to 2,147,483,647)
- Deep sleep current consumption: ~10-150 µA (depending on board and configuration)
- Wake-up latency: ~100-300ms from button press
- Boot counter stored in RTC memory survives deep sleep but not power loss

## Board Compatibility

This example is configured for Seeed XIAO ESP32S3 but can be adapted for other ESP32 boards by:
1. Changing the `board` setting in `platformio.ini`
2. Adjusting GPIO pins if needed
3. Modifying USB CDC flags if not using USB serial

## Technical Details

- Uses `nvs_flash.h` for NVS operations
- Uses `nvs.h` for NVS handle management
- Uses `esp_sleep.h` for deep sleep functionality
- Button wake-up using EXT0 (single GPIO, LOW level trigger)
- Timer wake-up using built-in RTC timer
- RTC memory for boot counter (survives deep sleep)
- Progress updates every second during button hold check
- Counter increments on each wake-up
- NVS operations: `nvs_open()`, `nvs_get_i32()`, `nvs_set_i32()`, `nvs_commit()`
- Deep sleep API: `esp_sleep_enable_ext0_wakeup()`, `esp_sleep_enable_timer_wakeup()`, `esp_deep_sleep_start()`
- Proper error handling with `esp_err_t` return codes

## Code Structure

- `readCounterFromNVS()` - Loads saved counter value from NVS
- `saveCounterToNVS()` - Saves current counter value to NVS
- `performFactoryReset()` - Erases NVS and restarts device
- `checkFactoryResetButton()` - Monitors button for 5-second hold on wake-up
- `printWakeupReason()` - Displays why device woke up (button/timer/first boot)
- `showNVSStats()` - Displays NVS partition statistics
- `setupDeepSleep()` - Configures wake-up sources (button + timer)
- `goToSleep()` - Enters deep sleep mode
- `setup()` - Handles wake-up, checks for reset, increments counter, sleeps
- `loop()` - Empty (never called due to deep sleep)

## Power Consumption

| State | Current | Notes |
|-------|---------|-------|
| Deep Sleep | ~10-150 µA | Minimal power, waiting for wake-up |
| Wake-up & Processing | ~30-80 mA | Brief active period (~3-5 seconds) |
| Factory Reset Check | ~30-80 mA | If button held (5 seconds) |

**Average Power**: Depends on wake-up frequency. With 10-second sleep cycles:
- Active: ~3 seconds every 10 seconds
- Average: ~10-15 mA (mostly sleep time contributes minimal current)


