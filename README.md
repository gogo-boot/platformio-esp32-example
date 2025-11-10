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

