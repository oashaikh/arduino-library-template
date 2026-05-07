/**
 * @file DebouncedButton.h
 *
 * Debounced active-low (or active-high) button reader.
 * Uses a millisecond clock injected at construction so it's testable without
 * Arduino — see test/host/test_DebouncedButton.cpp.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef ARDUINO
#include <Arduino.h>
#endif

namespace YourLibrary {

class DebouncedButton {
public:
    using Clock = uint32_t (*)();
    using Reader = bool (*)(uint8_t pin);

    /**
     * @param pin                 The GPIO pin number (only used by Reader).
     * @param activeLow           True if the button pulls the line low when pressed.
     * @param debounce_ms         Time the level must stay stable to count as a transition.
     * @param clock               Function returning monotonic ms (default: Arduino millis()).
     * @param reader              Function returning the raw level (default: Arduino digitalRead()).
     */
    DebouncedButton(uint8_t pin,
                    bool activeLow = true,
                    uint16_t debounce_ms = 25,
                    Clock clock = nullptr,
                    Reader reader = nullptr);

    /// Sample once. Call from your main loop.
    void update();

    /// True while the button is currently pressed (debounced).
    bool isPressed() const { return _stable_pressed; }

    /// True for one update() cycle after a press transition.
    bool wasPressed() {
        bool e = _press_event;
        _press_event = false;
        return e;
    }

    /// True for one update() cycle after a release transition.
    bool wasReleased() {
        bool e = _release_event;
        _release_event = false;
        return e;
    }

private:
    uint8_t  _pin;
    bool     _active_low;
    uint16_t _debounce_ms;
    Clock    _clock;
    Reader   _reader;

    bool     _last_raw_pressed = false;
    bool     _stable_pressed   = false;
    uint32_t _last_change_ms   = 0;
    bool     _press_event      = false;
    bool     _release_event    = false;
};

}  // namespace YourLibrary
