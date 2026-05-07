#include "test_runner.h"
#include "DebouncedButton.h"

using YourLibrary::DebouncedButton;

namespace {

uint32_t g_now = 0;
bool g_raw_high = true;       // active-low: HIGH = released

uint32_t fake_clock() { return g_now; }
bool fake_reader(uint8_t /*pin*/) { return g_raw_high; }

void reset() { g_now = 0; g_raw_high = true; }

}

TEST(initial_state_not_pressed) {
    reset();
    DebouncedButton b(2, true, 25, fake_clock, fake_reader);
    b.update();
    ASSERT_FALSE(b.isPressed());
}

TEST(transient_press_does_not_register) {
    reset();
    DebouncedButton b(2, true, 25, fake_clock, fake_reader);

    g_raw_high = false;       // pressed
    b.update();               // first dip — start counting
    g_now += 10;              // less than debounce
    g_raw_high = true;        // bounced back
    b.update();

    ASSERT_FALSE(b.isPressed());
    ASSERT_FALSE(b.wasPressed());
}

TEST(stable_press_registers_after_debounce) {
    reset();
    DebouncedButton b(2, true, 25, fake_clock, fake_reader);

    g_raw_high = false;       // pressed
    b.update();
    g_now += 30;              // > debounce
    b.update();

    ASSERT_TRUE(b.isPressed());
    ASSERT_TRUE(b.wasPressed());
    ASSERT_FALSE(b.wasPressed());   // edge consumed
}

TEST(release_event_fires_once) {
    reset();
    DebouncedButton b(2, true, 25, fake_clock, fake_reader);

    g_raw_high = false; b.update();
    g_now += 30;        b.update();
    (void)b.wasPressed();

    g_raw_high = true; b.update();
    g_now += 30;       b.update();

    ASSERT_FALSE(b.isPressed());
    ASSERT_TRUE(b.wasReleased());
    ASSERT_FALSE(b.wasReleased());
}

TEST(active_high_inverts_logic) {
    reset();
    DebouncedButton b(2, /*activeLow=*/false, 25, fake_clock, fake_reader);

    g_raw_high = true; b.update();
    g_now += 30;       b.update();

    ASSERT_TRUE(b.isPressed());
}
