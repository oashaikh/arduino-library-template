/*
 * Basic example: blink LED while debouncing a button on D2.
 */
#include <YourLibrary.h>

using namespace YourLibrary;

// Some boards (notably bare ESP32 dev boards) don't define LED_BUILTIN.
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

constexpr uint8_t LED_PIN    = LED_BUILTIN;
constexpr uint8_t BUTTON_PIN = 2;

DebouncedButton button(BUTTON_PIN, /*activeLow=*/true, /*debounce_ms=*/30);
RingBuffer<uint32_t, 16> events;

void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    while (!Serial && millis() < 2000) { /* wait for USB CDC */ }
    Serial.println(F("YourLibrary example: Basic"));
}

void loop() {
    button.update();

    if (button.wasPressed()) {
        events.push(millis());
        digitalWrite(LED_PIN, HIGH);
    }
    if (button.wasReleased()) {
        digitalWrite(LED_PIN, LOW);
    }

    uint32_t ts;
    while (events.pop(ts)) {
        Serial.print(F("press at t="));
        Serial.println(ts);
    }
}
