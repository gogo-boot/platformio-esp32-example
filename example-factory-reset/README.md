# Factory Reset Example - ESP32

This example demonstrates how to implement a factory reset function on ESP32 that erases all NVS (Non-Volatile Storage) data when a button is pressed and held for 5 seconds. It features a simple counter that increments every second and persists to NVS.

## Features

- ✨ Auto-incrementing counter stored in NVS
- 📊 Counter value printed every second ("Hello World 1", "Hello World 2", etc.)
- 💾 Persistent storage - counter survives reboots
- ⏱️ Detect long press (5 seconds) to trigger factory reset
- 🗑️ Erase all NVS stored data
- 🔄 Automatic restart after factory reset
- 📈 Counter resumes from last saved value after reboot
- 🔢 Counter resets to 0 after factory reset

## Hardware Requirements

- Seeed XIAO ESP32S3 (or any ESP32 board)
- Push button connected to GPIO 1 (with internal pull-up)

## Circuit

```
GPIO 1 ----[ Button ]---- GND
(Internal pull-up resistor enabled)
```

## How It Works

1. **Startup**: Device initializes NVS and loads the last saved counter value
2. **Counter Loop**: Every second:
   - Increments counter by 1
   - Prints "Hello World [counter]" to serial
   - Saves counter value to NVS
3. **Button Monitoring**: Continuously monitors reset button on GPIO 1
4. **Long Press Detection**: When button is held for 5 seconds:
   - Erases all NVS data
   - Reinitializes NVS
   - Restarts the device
5. **After Reset**: Counter starts from 0 again

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
🏭 FACTORY RESET EXAMPLE
🏭 ESP32 NVS Counter Demo
🏭 ================================

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 0
Free entries: 512
Total entries: 512
=================================

⚠️  No stored data found, starting from 0

📋 Instructions:
=================================
Counter increments every second and saves to NVS.
Press and HOLD button on GPIO 1 for 5 seconds
to erase NVS and reset counter to 0.
=================================

🎯 Ready! Counter started...

Hello World 1
Hello World 2
Hello World 3
Hello World 4
...
```

### After Reboot (Counter Resumes)
```
🏭 ================================
🏭 FACTORY RESET EXAMPLE
🏭 ESP32 NVS Counter Demo
🏭 ================================

✅ NVS initialized successfully
✅ Reset button configured on GPIO 1

📊 NVS Status:
=================================
Used entries: 1
Free entries: 511
Total entries: 512
=================================

✅ Loaded counter from NVS: 42

📋 Instructions:
=================================
Counter increments every second and saves to NVS.
Press and HOLD button on GPIO 1 for 5 seconds
to erase NVS and reset counter to 0.
=================================

🎯 Ready! Counter started...

Hello World 43
Hello World 44
Hello World 45
...
```

### Factory Reset Triggered
```
Hello World 100

🔵 Reset button pressed...
⏱️  Holding... 4 seconds remaining
⏱️  Holding... 3 seconds remaining
⏱️  Holding... 2 seconds remaining
⏱️  Holding... 1 seconds remaining

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
const int RESET_BUTTON_GPIO = 1;              // Reset button GPIO pin
const unsigned long HOLD_DURATION_MS = 5000;   // Hold time in milliseconds
const unsigned long COUNTER_INTERVAL = 1000;   // Counter increment interval
const char* NVS_NAMESPACE = "storage";         // NVS namespace
const char* NVS_COUNTER_KEY = "counter";       // NVS key for counter
```

## Use Cases

- Learn NVS read/write operations
- Implement persistent data storage
- Factory reset for production devices
- Clear all stored preferences and configuration data
- Testing NVS functionality
- Understanding ESP32 non-volatile storage

## Notes

- The button uses internal pull-up resistor (active LOW)
- Counter value is saved to NVS every second
- Counter persists across reboots
- Device automatically restarts after factory reset
- All data in NVS partition will be permanently erased after reset
- Counter uses `int32_t` type (range: -2,147,483,648 to 2,147,483,647)

## Board Compatibility

This example is configured for Seeed XIAO ESP32S3 but can be adapted for other ESP32 boards by:
1. Changing the `board` setting in `platformio.ini`
2. Adjusting GPIO pins if needed
3. Modifying USB CDC flags if not using USB serial

## Technical Details

- Uses `nvs_flash.h` for NVS operations
- Uses `nvs.h` for NVS handle management
- Implements button debouncing with 50ms delay
- Progress updates every second during button hold
- Counter increments and saves every 1 second
- NVS operations: `nvs_open()`, `nvs_get_i32()`, `nvs_set_i32()`, `nvs_commit()`
- Proper error handling with `esp_err_t` return codes

## Code Structure

- `readCounterFromNVS()` - Loads saved counter value from NVS
- `saveCounterToNVS()` - Saves current counter value to NVS
- `performFactoryReset()` - Erases NVS and restarts device
- `showNVSStats()` - Displays NVS partition statistics
- `setup()` - Initializes NVS, button, and loads counter
- `loop()` - Increments counter every second and monitors button


