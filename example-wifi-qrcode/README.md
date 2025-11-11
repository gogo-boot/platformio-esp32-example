# WiFi QR Code Generator Example

This example demonstrates how to generate a QR code for WiFi connection on ESP32 (XIAO ESP32S3).

## Features

- Generates QR code from WiFi credentials (SSID, Password, Security type)
- Displays QR code in the serial monitor using ASCII/Unicode blocks
- Supports WPA, WEP, and open network security types
- Uses the standard WiFi QR code format that most smartphones can scan

## Hardware Requirements

- Seeed XIAO ESP32S3 (or compatible ESP32 board)
- USB-C cable for programming and serial communication

## WiFi QR Code Format

The QR code follows the standard WiFi configuration format:

**For networks with password:**
```
WIFI:T:<security_type>;S:<ssid>;P:<password>;;
```

**For open networks (no password):**
```
WIFI:S:<ssid>;;
```

Where:
- `T` = Security type (WPA, WEP) - omitted for open networks
- `S` = Network SSID
- `P` = Password - omitted for open networks

Examples:
- With password: `WIFI:T:WPA;S:myssid;P:mypassword;;`
- Open network: `WIFI:S:FreeWiFi;;`

## Configuration

Edit the following constants in `src/main.cpp` to match your WiFi network:

**For password-protected networks:**
```cpp
const char* WIFI_SSID = "myssid";
const char* WIFI_PASSWORD = "mypassword";
const char* WIFI_SECURITY = "WPA";  // Options: WPA, WEP
```

**For open networks (no password):**
```cpp
const char* WIFI_SSID = "FreeWiFi";
const char* WIFI_PASSWORD = "";  // Empty string for open networks
const char* WIFI_SECURITY = "WPA";  // Ignored when password is empty
```

## How to Use

1. **Configure WiFi credentials**: Edit the constants at the top of `src/main.cpp`
2. **Build and upload**: 
   ```bash
   pio run -t upload
   ```
3. **Open serial monitor**:
   ```bash
   pio device monitor
   ```
4. **Scan the QR code**: Use your smartphone camera to scan the QR code displayed in the serial monitor
5. **Connect**: Your phone should automatically detect the WiFi network and prompt you to connect

## Tips for Scanning

- **Adjust terminal font size**: Make the QR code larger or smaller as needed for scanning
- **Increase contrast**: Use a dark terminal theme for better scanning
- **Distance**: Try different distances between your phone and the screen
- **Screen brightness**: Increase screen brightness for better recognition

## Library Used

This example uses the [QRCode library by ricmoo](https://github.com/ricmoo/qrcode) which is a lightweight QR code generator for embedded systems.

## Customization

### Change QR Code Version

If your WiFi string is too long (very long SSID or password), you can increase the QR code version:

```cpp
#define QR_VERSION 7  // Increase from 6 to 7 or higher
#define QR_BUFFER_SIZE 365  // Adjust buffer size accordingly
```

QR Code versions and their capacities (alphanumeric, ECC_LOW):
- Version 3: ~53 characters
- Version 4: ~78 characters
- Version 5: ~106 characters
- Version 6: ~134 characters
- Version 7: ~154 characters

### Error Correction Level

The example uses `ECC_LOW` for maximum data capacity. You can change to:
- `ECC_LOW` (7% error correction)
- `ECC_MEDIUM` (15% error correction)
- `ECC_QUARTILE` (25% error correction)
- `ECC_HIGH` (30% error correction)

Higher error correction allows the QR code to be read even if partially damaged, but reduces data capacity.

## Output Example

```
════════════════════════════════════════════
  ESP32 WiFi QR Code Generator
════════════════════════════════════════════

Configuration:
  SSID:     myssid
  Security: WPA
  Password: mypassword

WiFi Connection String:
WIFI:T:WPA;S:myssid;P:mypassword;;

✅ QR Code generated successfully!
QR Code size: 41 x 41 modules

[QR Code displayed here]

📱 Instructions:
1. Open your phone's camera app
2. Scan the QR code above from this serial monitor
3. Your phone should detect the WiFi network
4. Tap to connect automatically!
```

## Troubleshooting

**QR code won't scan:**
- Ensure the terminal background is dark and the QR code is white/bright
- Try adjusting your terminal font size
- Make sure your phone's camera app supports QR code scanning
- Try different screen brightness levels

**"String too long" error:**
- Increase the `QR_VERSION` value
- Update the `QR_BUFFER_SIZE` accordingly
- Consider using a shorter SSID or password

**Upload fails:**
- Hold the BOOT button while uploading if needed
- Ensure the correct port is selected
- Check USB cable connection

## License

This example is part of the platformio-esp32-example repository.

