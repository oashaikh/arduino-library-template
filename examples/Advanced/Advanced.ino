/*
 * Advanced example: ISR producer + main-loop consumer using RingBuffer.
 */
#include <YourLibrary.h>

constexpr uint8_t INT_PIN = 2;

RingBuffer<uint32_t, 32> events;

void IRAM_ATTR_OR_NOTHING() {
    // (Define IRAM_ATTR_OR_NOTHING per platform if needed.)
}

void onEdge() {
    events.push(micros());
}

void setup() {
    pinMode(INT_PIN, INPUT_PULLUP);
    Serial.begin(115200);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), onEdge, FALLING);
}

void loop() {
    uint32_t t;
    while (events.pop(t)) {
        Serial.print(F("edge at us="));
        Serial.println(t);
    }
}
