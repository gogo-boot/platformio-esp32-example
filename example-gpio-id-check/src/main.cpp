#include <Arduino.h>

// Seeed XIAO ESP32S3 available GPIOs
const int gpios[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 43, 44};
const int numGpios = sizeof(gpios) / sizeof(gpios[0]);

void setup()
{
    Serial.begin(115200);
    delay(2000); // Wait for USB CDC to initialize

    Serial.println("\n\n=================================");
    Serial.println("GPIO Button Detector - XIAO ESP32S3");
    Serial.println("=================================");
    Serial.println("Press any button to see its GPIO number");
    Serial.println();

    // Configure all GPIOs as input with pullup
    for (int i = 0; i < numGpios; i++) {
        pinMode(gpios[i], INPUT_PULLUP);
    }

    Serial.println("Monitoring GPIOs:");
    Serial.print("  ");
    for (int i = 0; i < numGpios; i++) {
        Serial.printf("%d ", gpios[i]);
    }
    Serial.println("\n");
}

void loop() {
    static bool lastState[sizeof(gpios)/sizeof(gpios[0])];
    static bool firstRun = true;

    // Initialize on first run
    if (firstRun) {
        for (int i = 0; i < numGpios; i++) {
            lastState[i] = digitalRead(gpios[i]);
        }
        firstRun = false;
    }

    // Check each GPIO for state change
    for (int i = 0; i < numGpios; i++) {
        bool currentState = digitalRead(gpios[i]);

        // Detect button press (HIGH to LOW transition)
        if (lastState[i] == HIGH && currentState == LOW) {
            Serial.printf("🔴 BUTTON PRESSED on GPIO %d\n", gpios[i]);
        }
        // Detect button release (LOW to HIGH transition)
        else if (lastState[i] == LOW && currentState == HIGH) {
            Serial.printf("🟢 BUTTON RELEASED on GPIO %d\n", gpios[i]);
        }

        lastState[i] = currentState;
    }

    delay(50); // Debounce delay
}
