# arduino-library-template

A drop-in **Arduino library** template that's accepted by Library Manager,
ships with examples that build for AVR / SAMD / ESP32 in CI, and includes
**host-side unit tests** so the algorithmic parts can be tested on a normal
laptop without flashing hardware.

## What this repo does

- Conforms to the Library Manager spec: `library.properties`, `keywords.txt`,
  `src/`, `examples/`, single root header.
- Ships two example types worth keeping:
  - `DebouncedButton` — testable design, takes injected clock + reader so
    host tests can simulate timing.
  - `RingBuffer<T, N>` — header-only fixed-capacity SPSC buffer.
- Host-side tests in `test/host/` use a tiny self-rolled test runner so
  there's no external dep — `make run` and you have results in seconds.
- CI matrix: AVR (Uno, Mega), SAMD (MKR), ESP32. Plus arduino-lint for
  Library Manager compliance.

## Project layout

- `library.properties` - Library Manager metadata.
- `keywords.txt` - IDE syntax colouring.
- `src/`
  - `YourLibrary.h` - main include.
  - `DebouncedButton.{h,cpp}` - example class with injected dependencies.
  - `RingBuffer.h` - header-only template.
- `examples/Basic/Basic.ino` - real Arduino sketch.
- `examples/Advanced/Advanced.ino` - ISR + ring buffer pattern.
- `test/host/` - host-side tests; runs with `make`.

## Quick start

Renaming for your library:

```bash
# Replace YourLibrary with your name everywhere:
grep -rl YourLibrary . --exclude-dir=.git | xargs sed -i 's/YourLibrary/MySensor/g'
mv src/YourLibrary.h src/MySensor.h
# Update library.properties: name, sentence, paragraph, includes
```

Try the host tests:

```bash
cd test/host && make run
```

Try a sketch:

```bash
arduino-cli core install arduino:avr
arduino-cli compile --fqbn arduino:avr:uno examples/Basic
```

Or open `examples/Basic/Basic.ino` in the Arduino IDE after symlinking the
repo into `~/Documents/Arduino/libraries/`.

## Testable embedded code

The trick that makes `DebouncedButton` testable on a laptop:

```cpp
DebouncedButton(uint8_t pin,
                bool activeLow,
                uint16_t debounce_ms,
                Clock clock,
                Reader reader);
```

`Clock` and `Reader` default to `millis` and `digitalRead` on Arduino, but
the host tests pass in fake versions so they can drive timing
deterministically. This same pattern works for any time- or IO-bound code.

## Publishing

When the library is ready:

1. Tag a release: `git tag v0.1.0 && git push --tags`.
2. (First time only) submit the repo to the [Arduino Library Manager
   index](https://github.com/arduino/library-registry).
3. Subsequent releases pick up automatically from new git tags.

## Why CI compiles examples

It's the only way to know if your library still compiles on every
supported architecture. AVR limits, ESP32 IRAM annotations, SAMD's lack of
`<atomic>` — they all surface here. Cheaper to find on every PR than after
a release.
