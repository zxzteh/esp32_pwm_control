#include "RingBuffer.h"

RingBuffer::RingBuffer() : 
    head_(0), 
    tail_(0), 
    count_(0) {}

void RingBuffer::push(uint8_t byte) {
    buffer_[head_] = byte;
    head_ = (head_ + 1) % maxSize;
    if (count_ < maxSize) {
        count_++;
    } else {
        //  overwrite oldest
        tail_ = (tail_ + 1) % maxSize;
    }
}

bool RingBuffer::pop(uint8_t &out) {
    if (count_ == 0) return false;
    out = buffer_[tail_];
    tail_ = (tail_ + 1) % maxSize;
    count_--;
    return true;
}

void RingBuffer::clear() {
    head_ = 0;
    tail_ = 0;
    count_ = 0;
}

bool RingBuffer::has_data() const {
    return (count_ > 0);
}

size_t RingBuffer::size() const {
    return count_;
}