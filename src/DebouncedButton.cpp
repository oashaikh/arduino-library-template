#include "DebouncedButton.h"

namespace YourLibrary {

#ifdef ARDUINO
static uint32_t arduino_millis() { return ::millis(); }
static bool     arduino_read(uint8_t pin) { return ::digitalRead(pin) == HIGH; }
#endif

DebouncedButton::DebouncedButton(uint8_t pin,
                                 bool activeLow,
                                 uint16_t debounce_ms,
                                 Clock clock,
                                 Reader reader)
    : _pin(pin)
    , _active_low(activeLow)
    , _debounce_ms(debounce_ms)
#ifdef ARDUINO
    , _clock(clock ? clock : &arduino_millis)
    , _reader(reader ? reader : &arduino_read)
#else
    , _clock(clock)
    , _reader(reader)
#endif
{}

void DebouncedButton::update()
{
    const bool raw_high   = _reader(_pin);
    const bool raw_pressed = _active_low ? !raw_high : raw_high;
    const uint32_t now    = _clock();

    if (raw_pressed != _last_raw_pressed) {
        _last_raw_pressed = raw_pressed;
        _last_change_ms   = now;
        return;
    }

    if (raw_pressed != _stable_pressed &&
        (now - _last_change_ms) >= _debounce_ms) {
        _stable_pressed = raw_pressed;
        if (raw_pressed) _press_event = true;
        else             _release_event = true;
    }
}

}  // namespace YourLibrary
