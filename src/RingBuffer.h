#pragma once

#include <stdint.h>
#include <stddef.h>

class RingBuffer {
public:
    RingBuffer();

    void push(uint8_t byte);
    bool pop(uint8_t &out);
    void clear();
    bool has_data() const;
    size_t size() const;

private:
    static constexpr size_t maxSize = 256;
    uint8_t buffer_[maxSize];
    size_t head_;
    size_t tail_;
    size_t count_;
};