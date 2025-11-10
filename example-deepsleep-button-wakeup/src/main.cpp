#include <Arduino.h>
#include <esp_sleep.h>

// Wake-up button GPIOs
const gpio_num_t BUTTON_GPIO_2 = GPIO_NUM_2;
const gpio_num_t BUTTON_GPIO_3 = GPIO_NUM_3;
const gpio_num_t BUTTON_GPIO_5 = GPIO_NUM_5;

// Sleep duration in seconds (only used if no button wakes up the device)
const int SLEEP_DURATION_SEC = 60;

// Boot count tracker
RTC_DATA_ATTR int bootCount = 0;

void printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    Serial.println("\n=================================");
    Serial.printf("Boot #%d\n", bootCount);
    Serial.println("=================================");

    switch(wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            Serial.println("🔵 Wakeup caused by external signal (RTC_IO)");
            break;
        case ESP_SLEEP_WAKEUP_EXT1: {
            Serial.println("🟢 Wakeup caused by button press!");
            uint64_t wakeup_pin_mask = esp_sleep_get_ext1_wakeup_status();

            if (wakeup_pin_mask & (1ULL << BUTTON_GPIO_2)) {
                Serial.println("   → Button on GPIO 2 was pressed");
            }
            if (wakeup_pin_mask & (1ULL << BUTTON_GPIO_3)) {
                Serial.println("   → Button on GPIO 3 was pressed");
            }
            if (wakeup_pin_mask & (1ULL << BUTTON_GPIO_5)) {
                Serial.println("   → Button on GPIO 5 was pressed");
            }
            break;
        }
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("⏰ Wakeup caused by timer");
            break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD:
            Serial.println("👆 Wakeup caused by touchpad");
            break;
        case ESP_SLEEP_WAKEUP_ULP:
            Serial.println("🔧 Wakeup caused by ULP program");
            break;
        default:
            Serial.println("⚡ First boot / Reset");
            break;
    }
}

void setupDeepSleep() {
    Serial.println("\n📋 Configuring deep sleep...");

    // Configure GPIOs as wake-up sources
    // ESP32-S3 uses ext1 for multiple GPIO wake-up
    // Wake up when ANY button is pressed (LOW level, since we use pull-up)
    uint64_t gpio_mask = (1ULL << BUTTON_GPIO_2) |
                         (1ULL << BUTTON_GPIO_3) |
                         (1ULL << BUTTON_GPIO_5);

    esp_sleep_enable_ext1_wakeup(gpio_mask, ESP_EXT1_WAKEUP_ANY_LOW);

    // Also enable timer wakeup as a backup
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION_SEC * 1000000ULL);

    Serial.printf("✓ Wake-up enabled on GPIOs: 2, 3, 5 (press button to wake)\n");
    Serial.printf("✓ Timer wake-up enabled: %d seconds\n", SLEEP_DURATION_SEC);
    Serial.println();
}

void goToSleep() {
    Serial.println("💤 Going to deep sleep now...");
    Serial.println("   Press button on GPIO 2, 3, or 5 to wake up!");
    Serial.println("=================================\n");
    Serial.flush(); // Wait for serial to finish
    delay(100);

    // Enter deep sleep
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(5000); // Wait for USB CDC to initialize

    // Increment boot counter
    ++bootCount;

    // Print wakeup reason
    printWakeupReason();

    // Configure buttons as inputs with pull-up
    pinMode(BUTTON_GPIO_2, INPUT_PULLUP);
    pinMode(BUTTON_GPIO_3, INPUT_PULLUP);
    pinMode(BUTTON_GPIO_5, INPUT_PULLUP);

    Serial.println("\n📊 Current button states:");
    Serial.printf("   GPIO 2: %s\n", digitalRead(BUTTON_GPIO_2) ? "HIGH (not pressed)" : "LOW (pressed)");
    Serial.printf("   GPIO 3: %s\n", digitalRead(BUTTON_GPIO_3) ? "HIGH (not pressed)" : "LOW (pressed)");
    Serial.printf("   GPIO 5: %s\n", digitalRead(BUTTON_GPIO_5) ? "HIGH (not pressed)" : "LOW (pressed)");

    // Setup deep sleep configuration
    setupDeepSleep();

    // Stay awake for a few seconds to allow serial monitoring
    Serial.println("⏱️  Device will sleep in 5 seconds...");
    for (int i = 3; i > 0; i--) {
        Serial.printf("   %d...\n", i);
        delay(1000);
    }

    // Go to deep sleep
    goToSleep();
}

void loop() {
    // This will never be called since we go to sleep in setup()
    // But keep it here for completeness
}
