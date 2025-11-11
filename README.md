# ESP32 Examples Collection

A collection of PlatformIO examples for ESP32 development, primarily targeting the Seeed Studio XIAO ESP32S3.

## Examples

### [example-gpio-id-check](./example-gpio-id-check/)
GPIO button detector to identify which pins are connected to buttons. Monitors all available GPIOs and reports when buttons are pressed/released.

**Features:**
- Monitors 12 GPIO pins simultaneously
- Real-time button press/release detection
- Built-in debouncing
- USB CDC serial output

### [example-deepsleep-button-wakeup](./example-deepsleep-button-wakeup/)
Deep sleep power management with GPIO button wake-up. Demonstrates ultra-low power mode with button-triggered wake-up on GPIOs 2, 3, and 5.

**Features:**
- Deep sleep mode (~10-150 µA current consumption)
- Wake-up from multiple GPIO buttons (2, 3, 5)
- Timer-based backup wake-up (60 seconds)
- Boot counter with RTC memory
- Wake-up reason detection

### [example-factory-reset](./example-factory-reset/)
Factory reset functionality with deep sleep power management. Press and hold button for 5 seconds during wake-up to erase NVS. Features a persistent counter that increments on each wake-up.

**Features:**
- Deep sleep mode (~10-150 µA current consumption)
- Auto-incrementing counter stored in NVS
- Counter persists across reboots and sleep cycles
- Wakes every 10 seconds OR on button press
- Long-press detection (5 seconds) on wake-up
- Complete NVS flash erase
- Simple demonstration of NVS + deep sleep

### [example-wifi-qrcode](./example-wifi-qrcode/)
WiFi QR Code generator that displays a scannable QR code in the serial monitor. Generates QR codes from WiFi credentials (SSID, password, security type) that can be scanned by smartphones to connect automatically.

**Features:**
- Generates WiFi QR codes using standard format
- Displays QR code in serial monitor with ASCII/Unicode blocks
- Supports WPA, WEP, and open network security
- Configurable QR code version and error correction
- Works with most smartphone camera apps


## Usage

Each example is a complete, standalone PlatformIO project. Navigate to any example directory and run:

```bash
cd example-gpio-id-check
pio run -t upload
pio device monitor
```

## Structure

Each example contains:
- `platformio.ini` - Project configuration
- `src/main.cpp` - Main source code
- `lib/` - Example-specific libraries
- `include/` - Example-specific headers
- `README.md` - Example documentation

## Hardware

Most examples are designed for:
- **Seeed Studio XIAO ESP32S3**
- Xtensa LX7 dual-core @ 240 MHz
- 8M PSRAM & 16MB Flash
- Bluetooth 5.0, WiFi

## Adding New Examples

1. Create a new directory: `example-your-name/`
2. Add complete PlatformIO structure (platformio.ini, src/, lib/, include/)
3. Add a README.md describing the example
4. Update this root README.md with the new example

## License

Each example may have its own license. Check individual example directories.

