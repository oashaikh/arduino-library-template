/**
 * @file RingBuffer.h
 *
 * Header-only fixed-capacity SPSC ring buffer. Safe for one-producer-one-consumer
 * use across an ISR/loop boundary on AVR/SAMD/ESP32 (uses volatile indices).
 */
#pragma once

#include <stdint.h>
#include <stddef.h>

template <typename T, size_t Capacity>
class RingBuffer {
public:
    bool push(const T& item) {
        size_t next = (_head + 1) % Capacity;
        if (next == _tail) return false;        // full
        _buffer[_head] = item;
        _head = next;
        return true;
    }

    bool pop(T& out) {
        if (_head == _tail) return false;       // empty
        out = _buffer[_tail];
        _tail = (_tail + 1) % Capacity;
        return true;
    }

    size_t size() const {
        return (_head + Capacity - _tail) % Capacity;
    }

    constexpr size_t capacity() const { return Capacity - 1; }
    bool isEmpty() const { return _head == _tail; }
    bool isFull()  const { return ((_head + 1) % Capacity) == _tail; }

    void clear() { _head = _tail = 0; }

private:
    T _buffer[Capacity];
    volatile size_t _head = 0;
    volatile size_t _tail = 0;
};
