# ESP32 Examples Collection

A collection of PlatformIO examples for ESP32 development, primarily targeting the Seeed Studio XIAO ESP32S3.

## Quick Start

### Command Line

```bash
# Build a specific example
pio run -e factory-reset

# Upload to device
pio run -e factory-reset -t upload

# Monitor serial output
pio device monitor

# Build all examples at once
pio run
```

### CLion IDE

1. **Open** the root directory in CLion
2. **Select** environment from dropdown (e.g., `PlatformIO > factory-reset`)
3. **Build/Upload** using toolbar buttons

**Configure once, use for all examples!** ✨

## Available Examples

| Environment | Description |
|-------------|-------------|
| `gpio-id-check` | GPIO button detector to identify pins |
| `deepsleep-button-wakeup` | Deep sleep with GPIO wake-up |
| `factory-reset` | NVS factory reset with long-press |
| `wifi-qrcode` | WiFi QR code generator |

### Example 1: GPIO ID Check

Button detector to identify which GPIO pins are connected to buttons.

```bash
# Build and upload
pio run -e gpio-id-check -t upload

# Monitor output
pio device monitor
```

**Features:**
- Monitors 12 GPIO pins simultaneously
- Real-time button press/release detection
- Built-in debouncing
- USB CDC serial output

📄 [Detailed documentation](./example-gpio-id-check/)

### Example 2: Deep Sleep Button Wakeup

Deep sleep power management with GPIO button wake-up on pins 2, 3, and 5.

```bash
# Build and upload
pio run -e deepsleep-button-wakeup -t upload

# Monitor output
pio device monitor
```

**Features:**
- Deep sleep mode (~10-150 µA current consumption)
- Wake from multiple GPIO buttons (2, 3, 5)
- Timer-based backup wake-up (60 seconds)
- Boot counter with RTC memory
- Wake-up reason detection

📄 [Detailed documentation](./example-deepsleep-button-wakeup/)

### Example 3: Factory Reset

Factory reset functionality with NVS storage and deep sleep. Press and hold button for 5 seconds to erase.

```bash
# Build and upload
pio run -e factory-reset -t upload

# Monitor output
pio device monitor
```

**Features:**
- Deep sleep mode (~10-150 µA)
- Auto-incrementing counter in NVS
- Counter persists across reboots
- Wakes every 10 seconds OR on button press
- Long-press detection (5 seconds)
- Complete NVS flash erase

📄 [Detailed documentation](./example-factory-reset/)

### Example 4: WiFi QR Code Generator

Generate WiFi QR codes that can be scanned by smartphones for instant connection.

```bash
# Edit WiFi credentials first
# Open src/wifi-qrcode/main.cpp and set your SSID/password

# Build and upload
pio run -e wifi-qrcode -t upload

# Monitor to see QR code
pio device monitor
```

**Features:**
- Generates WiFi QR codes using standard format
- Displays QR code in serial monitor with ASCII/Unicode blocks
- Supports WPA, WEP, and open network security
- Simplified format for open networks (`WIFI:S:ssid;;`)
- Works with most smartphone camera apps

📄 [Detailed documentation](./example-wifi-qrcode/)

## Project Structure

```
platformio-esp32-example/
├── platformio.ini           # Unified configuration for all examples
├── src/                     # All example source code
│   ├── gpio-id-check/
│   │   └── main.cpp
│   ├── deepsleep-button-wakeup/
│   │   └── main.cpp
│   ├── factory-reset/
│   │   └── main.cpp
│   └── wifi-qrcode/
│       └── main.cpp
├── include/                 # Shared headers
├── lib/                     # Shared libraries
├── test/                    # Tests
├── example-*/              # Original examples (docs/reference)
├── UNIFIED_PROJECT.md      # Setup guide
└── CLION_GUIDE.md          # CLion quick reference
```

Each environment builds its corresponding `src/` subdirectory independently.

## Switching Between Examples

### Command Line
```bash
pio run -e gpio-id-check          # GPIO button detector
pio run -e deepsleep-button-wakeup # Deep sleep with wake-up
pio run -e factory-reset           # Factory reset demo
pio run -e wifi-qrcode             # WiFi QR code generator
```

### CLion
Use the environment dropdown in the top-right corner:
- `PlatformIO > gpio-id-check`
- `PlatformIO > deepsleep-button-wakeup`
- `PlatformIO > factory-reset`
- `PlatformIO > wifi-qrcode`

## Hardware

All examples are designed for:
- **Seeed Studio XIAO ESP32S3**
- Xtensa LX7 dual-core @ 240MHz
- 8MB PSRAM & 16MB Flash
- Bluetooth 5.0, WiFi
- USB-C connector

## Adding New Examples

1. Create a new subdirectory in `src/`:
   ```bash
   mkdir src/my-new-example
   ```

2. Add your code:
   ```bash
   # Create main.cpp
   touch src/my-new-example/main.cpp
   ```

3. Add environment to `platformio.ini`:
   ```ini
   [env:my-new-example]
   lib_deps = 
       # Add required libraries here
   build_src_filter = 
       +<my-new-example/>
       -<gpio-id-check/>
       -<deepsleep-button-wakeup/>
       -<factory-reset/>
       -<wifi-qrcode/>
   ```

4. Build it:
   ```bash
   pio run -e my-new-example
   ```

See [UNIFIED_PROJECT.md](./UNIFIED_PROJECT.md) for more details.

## Adding New Examples

1. Create a new directory: `example-your-name/`
2. Add complete PlatformIO structure (platformio.ini, src/, lib/, include/)
3. Add a README.md describing the example
4. Update this root README.md with the new example

## License

Each example may have its own license. Check individual example directories.

