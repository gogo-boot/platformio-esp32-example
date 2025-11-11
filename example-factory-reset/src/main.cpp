#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <esp_sleep.h>

// Reset button GPIO
const gpio_num_t RESET_BUTTON_GPIO = GPIO_NUM_1;

// Hold duration in milliseconds to trigger factory reset
const unsigned long HOLD_DURATION_MS = 5000;

// Sleep duration in seconds
const int SLEEP_DURATION_SEC = 10;

// Wake duration - how long to stay awake for counter update
const unsigned long AWAKE_DURATION_MS = 3000;

// NVS namespace and key
const char* NVS_NAMESPACE = "storage";
const char* NVS_COUNTER_KEY = "counter";

// Counter value
int32_t counter = 0;

// Boot count tracker
RTC_DATA_ATTR int bootCount = 0;


int32_t readCounterFromNVS() {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs_handle);
    if (err != ESP_OK) {
        Serial.println("⚠️  No stored data found, starting from 0");
        return 0;
    }

    // Read counter
    int32_t value = 0;
    err = nvs_get_i32(nvs_handle, NVS_COUNTER_KEY, &value);
    nvs_close(nvs_handle);

    if (err != ESP_OK) {
        Serial.println("⚠️  No counter value found, starting from 0");
        return 0;
    }

    Serial.printf("✅ Loaded counter from NVS: %d\n", value);
    return value;
}

void saveCounterToNVS(int32_t value) {
    nvs_handle_t nvs_handle;
    esp_err_t err;

    // Open NVS
    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        Serial.printf("❌ Error opening NVS: %s\n", esp_err_to_name(err));
        return;
    }

    // Write counter
    err = nvs_set_i32(nvs_handle, NVS_COUNTER_KEY, value);
    if (err != ESP_OK) {
        Serial.printf("❌ Error saving counter: %s\n", esp_err_to_name(err));
    }

    // Commit
    err = nvs_commit(nvs_handle);
    if (err != ESP_OK) {
        Serial.printf("❌ Error committing NVS: %s\n", esp_err_to_name(err));
    }

    nvs_close(nvs_handle);
}

void performFactoryReset() {
    Serial.println("\n🔥 ================================");
    Serial.println("🔥 FACTORY RESET INITIATED!");
    Serial.println("🔥 ================================\n");

    Serial.println("🗑️  Erasing NVS (Non-Volatile Storage)...");
    esp_err_t err = nvs_flash_erase();
    if (err == ESP_OK) {
        Serial.println("✅ NVS erased successfully!");

        // Reinitialize NVS
        err = nvs_flash_init();
        if (err == ESP_OK) {
            Serial.println("✅ NVS reinitialized successfully!");
        } else {
            Serial.printf("⚠️  NVS reinitialization failed: %s\n", esp_err_to_name(err));
        }
    } else {
        Serial.printf("❌ NVS erase failed: %s\n", esp_err_to_name(err));
    }

    Serial.println("\n✨ Factory reset complete!");
    Serial.println("   Counter will start from 0 again.");

    Serial.println("\n🔄 Restarting device in 3 seconds...");
    delay(3000);

    // Restart the device
    ESP.restart();
}

bool checkFactoryResetButton() {
    // Check if button is currently pressed
    if (digitalRead(RESET_BUTTON_GPIO) == LOW) {
        Serial.println("🔵 Reset button detected on wake-up!");
        Serial.println("   Hold button for 5 seconds to factory reset...");

        unsigned long startTime = millis();

        // Monitor button for 5 seconds
        while (millis() - startTime < HOLD_DURATION_MS) {
            // Check if button was released
            if (digitalRead(RESET_BUTTON_GPIO) == HIGH) {
                unsigned long heldDuration = millis() - startTime;
                Serial.printf("🟢 Button released after %.1f seconds\n", heldDuration / 1000.0);
                Serial.println("   (Not long enough for factory reset)\n");
                return false;
            }

            // Show progress every second
            unsigned long elapsed = millis() - startTime;
            static unsigned long lastProgressTime = 0;
            if (elapsed - lastProgressTime >= 1000) {
                unsigned long secondsRemaining = (HOLD_DURATION_MS - elapsed) / 1000;
                if (secondsRemaining > 0) {
                    Serial.printf("⏱️  Holding... %lu seconds remaining\n", secondsRemaining);
                }
                lastProgressTime = elapsed;
            }

            delay(50); // Small delay for debouncing
        }

        // Button was held for full 5 seconds
        Serial.println("✅ Button held for 5 seconds!");
        return true;
    }

    return false;
}

void printWakeupReason() {
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    Serial.println("\n=================================");
    Serial.printf("Boot #%d\n", bootCount);
    Serial.println("=================================");

    switch(wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0: {
            Serial.println("🟢 Wakeup caused by button press!");
            break;
        }
        case ESP_SLEEP_WAKEUP_TIMER:
            Serial.println("⏰ Wakeup caused by timer");
            break;
        default:
            Serial.println("⚡ First boot / Reset");
            break;
    }
}

void showNVSStats() {
    Serial.println("\n📊 NVS Status:");
    Serial.println("=================================");

    nvs_stats_t nvs_stats;
    esp_err_t err = nvs_get_stats(NULL, &nvs_stats);
    if (err == ESP_OK) {
        Serial.printf("Used entries: %d\n", nvs_stats.used_entries);
        Serial.printf("Free entries: %d\n", nvs_stats.free_entries);
        Serial.printf("Total entries: %d\n", nvs_stats.total_entries);
    } else {
        Serial.printf("Failed to get NVS stats: %s\n", esp_err_to_name(err));
    }
    Serial.println("=================================\n");
}

void setupDeepSleep() {
    Serial.println("\n📋 Configuring deep sleep...");

    // Configure button as wake-up source (EXT0 - single GPIO, LOW level)
    esp_sleep_enable_ext0_wakeup(RESET_BUTTON_GPIO, 0); // 0 = LOW level

    // Also enable timer wakeup
    esp_sleep_enable_timer_wakeup(SLEEP_DURATION_SEC * 1000000ULL);

    Serial.printf("✓ Wake-up enabled on GPIO %d (button press)\n", RESET_BUTTON_GPIO);
    Serial.printf("✓ Timer wake-up enabled: %d seconds\n", SLEEP_DURATION_SEC);
    Serial.println();
}

void goToSleep() {
    Serial.println("💤 Going to deep sleep now...");
    Serial.printf("   Will wake in %d seconds or when button is pressed\n", SLEEP_DURATION_SEC);
    Serial.println("   Hold button for 5 seconds during wake to factory reset!");
    Serial.println("=================================\n");
    Serial.flush(); // Wait for serial to finish
    delay(100);

    // Enter deep sleep
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB CDC to initialize

    // Increment boot counter
    ++bootCount;

    Serial.println("\n\n🏭 ================================");
    Serial.println("🏭 FACTORY RESET + DEEP SLEEP");
    Serial.println("🏭 ESP32 NVS Counter Demo");
    Serial.println("🏭 ================================\n");

    // Print wakeup reason
    printWakeupReason();

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        Serial.println("⚠️  NVS partition was truncated, erasing...");
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    if (err != ESP_OK) {
        Serial.printf("❌ NVS initialization failed: %s\n", esp_err_to_name(err));
    } else {
        Serial.println("✅ NVS initialized successfully");
    }

    // Configure reset button
    pinMode(RESET_BUTTON_GPIO, INPUT_PULLUP);
    Serial.printf("✅ Reset button configured on GPIO %d\n", RESET_BUTTON_GPIO);

    // Show current NVS status
    showNVSStats();

    // **CHECK FOR FACTORY RESET ON WAKE-UP**
    if (checkFactoryResetButton()) {
        // Button was held for 5 seconds, perform factory reset
        performFactoryReset();
        // Note: performFactoryReset() calls ESP.restart(), so we never reach here
    }

    // Load counter from NVS
    counter = readCounterFromNVS();

    // Increment counter
    counter++;
    Serial.printf("\n📊 Hello World %d\n", counter);
    saveCounterToNVS(counter);

    Serial.println("\n📋 How it works:");
    Serial.println("=================================");
    Serial.println("Device sleeps for 10 seconds between counts.");
    Serial.println("Wake up by pressing button OR timer.");
    Serial.printf("HOLD button for 5 seconds to factory reset!\n");
    Serial.println("=================================\n");

    // Setup deep sleep configuration
    setupDeepSleep();

    // Stay awake for a few seconds to allow serial monitoring
    Serial.println("⏱️  Device will sleep in 3 seconds...");
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

