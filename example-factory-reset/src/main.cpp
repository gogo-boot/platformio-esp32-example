#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>

// Reset button GPIO
const int RESET_BUTTON_GPIO = 1;

// Hold duration in milliseconds to trigger factory reset
const unsigned long HOLD_DURATION_MS = 5000;

// NVS namespace and key
const char* NVS_NAMESPACE = "storage";
const char* NVS_COUNTER_KEY = "counter";

// Variables for button state tracking
unsigned long buttonPressStartTime = 0;
bool buttonWasPressed = false;
bool factoryResetTriggered = false;

// Counter value
int32_t counter = 0;

// Timer for counter increment
unsigned long lastCounterUpdate = 0;
const unsigned long COUNTER_INTERVAL = 1000; // 1 second


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

void setup() {
    Serial.begin(115200);
    delay(2000); // Wait for USB CDC to initialize

    Serial.println("\n\n🏭 ================================");
    Serial.println("🏭 FACTORY RESET EXAMPLE");
    Serial.println("🏭 ESP32 NVS Counter Demo");
    Serial.println("🏭 ================================\n");

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

    // Load counter from NVS
    counter = readCounterFromNVS();

    Serial.println("📋 Instructions:");
    Serial.println("=================================");
    Serial.println("Counter increments every second and saves to NVS.");
    Serial.printf("Press and HOLD button on GPIO %d for 5 seconds\n", RESET_BUTTON_GPIO);
    Serial.println("to erase NVS and reset counter to 0.");
    Serial.println("=================================\n");

    Serial.println("🎯 Ready! Counter started...\n");
}

void loop() {
    // Increment counter every second
    if (millis() - lastCounterUpdate >= COUNTER_INTERVAL) {
        counter++;
        Serial.printf("Hello World %d\n", counter);
        saveCounterToNVS(counter);
        lastCounterUpdate = millis();
    }

    // Check reset button
    bool currentButtonState = digitalRead(RESET_BUTTON_GPIO);

    // Button is pressed (LOW because of INPUT_PULLUP)
    if (currentButtonState == LOW) {
        // Button just pressed
        if (!buttonWasPressed) {
            buttonWasPressed = true;
            buttonPressStartTime = millis();
            Serial.println("\n🔵 Reset button pressed...");
        }
        // Button is being held
        else {
            unsigned long holdDuration = millis() - buttonPressStartTime;

            // Show progress every second
            static unsigned long lastProgressTime = 0;
            if (millis() - lastProgressTime >= 1000) {
                unsigned long secondsHeld = holdDuration / 1000;
                unsigned long secondsRemaining = (HOLD_DURATION_MS / 1000) - secondsHeld;

                if (secondsRemaining > 0) {
                    Serial.printf("⏱️  Holding... %lu seconds remaining\n", secondsRemaining);
                }
                lastProgressTime = millis();
            }

            // Check if hold duration met
            if (holdDuration >= HOLD_DURATION_MS && !factoryResetTriggered) {
                factoryResetTriggered = true;
                performFactoryReset();
            }
        }
    }
    // Button released
    else {
        if (buttonWasPressed) {
            unsigned long holdDuration = millis() - buttonPressStartTime;

            if (!factoryResetTriggered) {
                Serial.printf("🟢 Reset button released (held for %.1f seconds)\n", holdDuration / 1000.0);
                if (holdDuration < HOLD_DURATION_MS) {
                    Serial.println("   (Not long enough for factory reset)\n");
                }
            }

            buttonWasPressed = false;
            factoryResetTriggered = false;
        }
    }

    delay(50); // Debounce delay
}

