#include <Arduino.h>
#include "qrcode.h"

// WiFi credentials - modify these for your network
const char* WIFI_SSID = "myssid";
const char* WIFI_PASSWORD = "mypassword";  // Set to "" or nullptr for open networks
const char* WIFI_SECURITY = "WPA"; // Options: WPA, WEP (ignored if password is empty)

// QR Code configuration
#define QR_VERSION 6  // Version 6 can hold up to ~100 characters
#define QR_BUFFER_SIZE 264  // Buffer size for version 6

void printQRCode(QRCode *qrcode) {
    Serial.println("\n\n");
    Serial.println("╔════════════════════════════════════════════╗");
    Serial.println("║        WiFi QR Code Generator              ║");
    Serial.println("╚════════════════════════════════════════════╝");
    Serial.println();

    // Print QR code with border
    // Top border
    for (uint8_t x = 0; x < qrcode->size + 4; x++) {
        Serial.print("██");
    }
    Serial.println();

    for (uint8_t x = 0; x < qrcode->size + 4; x++) {
        Serial.print("██");
    }
    Serial.println();

    // QR code content with side borders
    for (uint8_t y = 0; y < qrcode->size; y++) {
        Serial.print("████"); // Left border (2 blocks)

        for (uint8_t x = 0; x < qrcode->size; x++) {
            if (qrcode_getModule(qrcode, x, y)) {
                Serial.print("██");  // Black module
            } else {
                Serial.print("  ");  // White module
            }
        }

        Serial.print("████"); // Right border (2 blocks)
        Serial.println();
    }

    // Bottom border
    for (uint8_t x = 0; x < qrcode->size + 4; x++) {
        Serial.print("██");
    }
    Serial.println();

    for (uint8_t x = 0; x < qrcode->size + 4; x++) {
        Serial.print("██");
    }
    Serial.println();

    Serial.println();
}

void generateWiFiQRCode(const char* ssid, const char* password, const char* security) {
    // Allocate memory for QR code
    uint8_t qrcodeData[QR_BUFFER_SIZE];
    QRCode qrcode;

    // Build WiFi connection string
    // Format with password: WIFI:T:<security>;S:<ssid>;P:<password>;;
    // Format without password: WIFI:S:<ssid>;;
    String wifiString = "WIFI:";

    // Check if password is empty or null
    bool hasPassword = (password != nullptr && strlen(password) > 0);

    if (hasPassword) {
        // Include security type and password
        wifiString += "T:";
        wifiString += security;
        wifiString += ";";
    }

    wifiString += "S:";
    wifiString += ssid;
    wifiString += ";";

    if (hasPassword) {
        wifiString += "P:";
        wifiString += password;
        wifiString += ";";
    }

    wifiString += ";";

    Serial.println("WiFi Connection String:");
    Serial.println(wifiString);
    Serial.println();

    Serial.printf("String length: %d characters\n", wifiString.length());
    Serial.println();

    // Generate QR code
    int8_t result = qrcode_initText(&qrcode, qrcodeData, QR_VERSION, ECC_LOW, wifiString.c_str());

    if (result == 0) {
        Serial.println("✅ QR Code generated successfully!");
        Serial.printf("QR Code size: %d x %d modules\n", qrcode.size, qrcode.size);
        printQRCode(&qrcode);

        Serial.println("\n📱 Instructions:");
        Serial.println("1. Open your phone's camera app");
        Serial.println("2. Scan the QR code above from this serial monitor");
        Serial.println("3. Your phone should detect the WiFi network");
        Serial.println("4. Tap to connect automatically!");
        Serial.println();
        Serial.println("💡 Tip: You may need to adjust your terminal/serial");
        Serial.println("   monitor font size to scan the QR code properly.");
    } else {
        Serial.printf("❌ Error generating QR code: %d\n", result);
        Serial.println("The WiFi string might be too long for this QR version.");
        Serial.println("Try a shorter SSID or password, or increase QR_VERSION.");
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB CDC to initialize

    Serial.println("\n\n");
    Serial.println("════════════════════════════════════════════");
    Serial.println("  ESP32 WiFi QR Code Generator");
    Serial.println("════════════════════════════════════════════");
    Serial.println();

    Serial.println("Configuration:");
    Serial.printf("  SSID:     %s\n", WIFI_SSID);
    Serial.printf("  Security: %s\n", WIFI_SECURITY);
    Serial.printf("  Password: %s\n", WIFI_PASSWORD);
    Serial.println();

    // Generate and display QR code
    generateWiFiQRCode(WIFI_SSID, WIFI_PASSWORD, WIFI_SECURITY);

    Serial.println("\n════════════════════════════════════════════");
    Serial.println("To generate a new QR code with different");
    Serial.println("credentials, modify the constants at the");
    Serial.println("top of main.cpp and upload again.");
    Serial.println("════════════════════════════════════════════");
}

void loop() {
    // Nothing to do in loop - QR code is generated once in setup()
    delay(1000);
}

