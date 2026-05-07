#include "test_runner.h"
#include "RingBuffer.h"

using YourLibrary::RingBuffer;

TEST(ring_buffer_starts_empty) {
    RingBuffer<int, 8> rb;
    ASSERT_TRUE(rb.isEmpty());
    ASSERT_EQ(rb.size(), 0u);
    ASSERT_EQ(rb.capacity(), 7u);   // capacity = N - 1 (one slot reserved)
}

TEST(ring_buffer_push_pop_fifo) {
    RingBuffer<int, 8> rb;
    for (int i = 0; i < 5; ++i) ASSERT_TRUE(rb.push(i));
    ASSERT_EQ(rb.size(), 5u);

    int out = -1;
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(rb.pop(out));
        ASSERT_EQ(out, i);
    }
    ASSERT_TRUE(rb.isEmpty());
}

TEST(ring_buffer_full_returns_false) {
    RingBuffer<int, 4> rb;       // capacity 3
    ASSERT_TRUE(rb.push(1));
    ASSERT_TRUE(rb.push(2));
    ASSERT_TRUE(rb.push(3));
    ASSERT_FALSE(rb.push(4));    // full
    ASSERT_TRUE(rb.isFull());
}

TEST(ring_buffer_pop_after_wraparound) {
    RingBuffer<int, 4> rb;       // capacity 3
    rb.push(1); rb.push(2); rb.push(3);
    int x;
    rb.pop(x); rb.pop(x);        // free 2 slots
    rb.push(4); rb.push(5);

    ASSERT_TRUE(rb.pop(x)); ASSERT_EQ(x, 3);
    ASSERT_TRUE(rb.pop(x)); ASSERT_EQ(x, 4);
    ASSERT_TRUE(rb.pop(x)); ASSERT_EQ(x, 5);
    ASSERT_TRUE(rb.isEmpty());
}

TEST(ring_buffer_clear_resets) {
    RingBuffer<int, 8> rb;
    rb.push(1); rb.push(2);
    rb.clear();
    ASSERT_TRUE(rb.isEmpty());
}
