#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <gdey/GxEPD2_750_GDEY075T7.h>  // Specific driver for GDEY075T7
#include "bitmaps/Bitmaps800x480.h" // 7.5"  b/w
#include "bitmaps/Bitmaps168x384.h"

#define PIN_EPD_PWR 9
RTC_DATA_ATTR int initCount = 0;

// GxEPD2_750_GDEY075T7 epd(/*cs=*/ 3, /*dc=*/ 9, /*rst=*/ 8, /*busy=*/ 2);
GxEPD2_750_GDEY075T7 epd(/*cs=*/ 44, /*dc=*/ 10, /*rst=*/ 38, /*busy=*/ 4);
GxEPD2_BW<GxEPD2_750_GDEY075T7, GxEPD2_750_GDEY075T7::HEIGHT> display(epd);
U8G2_FOR_ADAFRUIT_GFX u8g2;

int initDisplay() {
    bool initial = initCount == 0;
    initCount++;
    pinMode(PIN_EPD_PWR, OUTPUT);
    digitalWrite(PIN_EPD_PWR, HIGH);
    display.init(115200, initial, 2, false);

    u8g2.begin(display);
    u8g2.setFontMode(1); // Use u8g2 transparent mode
    u8g2.setFont(u8g2_font_helvB10_tf); 
    u8g2.setFontDirection(0); // Left to right
    u8g2.setForegroundColor(GxEPD_BLACK);
    u8g2.setBackgroundColor(GxEPD_WHITE);

    return initCount;
}

void powerOff() {
    display.hibernate();
    digitalWrite(PIN_EPD_PWR, LOW); // Power off the display
}

void deepSleep() {
    uint64_t sleepDuration = 0.1 * 60ULL; // ~6s
    esp_sleep_enable_timer_wakeup(sleepDuration * 1000000ULL);
    esp_deep_sleep_start();
}

void setup()
{
    int wakeups = initDisplay();

    String hello = "ÄÖÜäöüHelloWorld";
    uint16_t textWidth = u8g2.getUTF8Width(hello.c_str());
    uint16_t fontAscent = u8g2.getFontAscent();
    int16_t fontDescent = u8g2.getFontDescent();
    uint16_t totalHeight = u8g2.getFontAscent() - u8g2.getFontDescent();
    //Font size: width=76, ascent=11, descent=-3, totalHeight=14
    Serial.printf("Font size: width=%u, ascent=%u, descent=%d, totalHeight=%u\n", textWidth, fontAscent, fontDescent, totalHeight);

    if (wakeups == 1) {
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);
            display.drawBitmap(0, 0, Bitmap800x480_2, 800, 480, GxEPD_BLACK);
        } while (display.nextPage());
    } else {

        display.setPartialWindow(0, 0, textWidth, totalHeight);
        display.firstPage();
        do {
            display.fillScreen(GxEPD_WHITE);
            u8g2.setCursor(0, fontAscent );
            u8g2.print(hello.c_str());
        } while (display.nextPage());
    }
    powerOff();
    deepSleep();
}

void loop() {}

