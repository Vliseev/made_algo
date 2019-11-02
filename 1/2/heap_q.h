#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>
#include <vector>

template <typename _Tp, typename _Compare = std::less<_Tp>>
class HeapQ {
    using ValueType = _Tp;

    void SiftDown(size_t idx, const _Compare &comp);
    void SiftUp(size_t idx, const _Compare &comp);
    void BuildHeap();
    void AddEll(const _Tp &el);
    void AddEll(_Tp &&el);

   public:
    HeapQ();
    explicit HeapQ(const std::vector<_Tp> &);
    explicit HeapQ(std::vector<_Tp> &&);
    HeapQ(const HeapQ &);
    HeapQ(HeapQ &&) noexcept;
    HeapQ &operator=(const HeapQ &);
    HeapQ &operator=(HeapQ &&) noexcept;

    void Add(_Tp &&el);
    void Add(const _Tp &el);
    const _Tp &Top() const;
    void Pop();
    ~HeapQ() {
        delete[] buffer_;
    }

    bool Empty() {
        return size_ == 0;
    }

   private:
    ValueType *buffer_;
    size_t size_;
    size_t capacity_;
};

template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare>::HeapQ()
    : capacity_(4), size_(0), buffer_(new ValueType[capacity_]) {
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::SiftDown(size_t idx, const _Compare &comp) {
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;

    size_t largest = idx;
    if (left < size_ && comp(buffer_[idx], buffer_[left]))
        largest = left;
    if (right < size_ && comp(buffer_[largest], buffer_[right]))
        largest = right;

    if (largest != idx) {
        std::swap(buffer_[idx], buffer_[largest]);
        SiftDown(largest, comp);
    }
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::SiftUp(size_t idx, const _Compare &comp) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (!comp(buffer_[parent], buffer_[idx]))
            return;
        std::swap(buffer_[idx], buffer_[parent]);
        idx = parent;
    }
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::BuildHeap() {
    _Compare cmp;
    for (int64_t i = size_ / 2 - 1; i >= 0; --i) {
        SiftDown(i, cmp);
    }
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::AddEll(const _Tp &el) {
    if (size_ == capacity_) {
        capacity_ *= 2;
        auto tmp_buf = new ValueType[capacity_];
        std::move(buffer_, buffer_ + size_, tmp_buf);
        delete[] buffer_;
        buffer_ = tmp_buf;
    }
    buffer_[size_++] = el;
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::AddEll(_Tp &&el) {
    if (size_ == capacity_) {
        capacity_ *= 2;
        auto tmp_buf = new ValueType[capacity_];
        std::move(buffer_, buffer_ + size_, tmp_buf);
        delete[] buffer_;
        buffer_ = tmp_buf;
    }
    buffer_[++size_] = std::move(el);
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::Add(_Tp &&el) {
    _Compare cmp;
    AddEll(std::move(el));
    SiftUp(size_ - 1, cmp);
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::Add(const _Tp &el) {
    _Compare cmp;
    AddEll(el);
    SiftUp(size_ - 1, cmp);
}

template <typename _Tp, typename _Compare>
const _Tp &HeapQ<_Tp, _Compare>::Top() const {
    assert(size_ != 0);
    return buffer_[0];
}

template <typename _Tp, typename _Compare>
void HeapQ<_Tp, _Compare>::Pop() {
    _Compare cmp;
    assert(size_ != 0);

    buffer_[0] = buffer_[--size_];
    if (size_)
        SiftDown(0, cmp);
}

template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare>::HeapQ(const std::vector<_Tp> &v) {
    size_ = v.size();
    capacity_ = v.capacity();
    buffer_ = new ValueType[capacity_];

    std::copy(v.begin(), v.end(), buffer_);
    BuildHeap();
}

template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare>::HeapQ(std::vector<_Tp> &&v) {
    size_ = v.size();
    capacity_ = v.capacity();
    buffer_ = new ValueType[capacity_];

    std::move(v.begin(), v.end(), buffer_);
    BuildHeap();
}
template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare>::HeapQ(const HeapQ &other) {
    if (other.buffer_) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = new ValueType[capacity_];
        std::copy(std::begin(other.buffer_), std::begin(other.buffer_) + size_,
                  std::begin(buffer_));
    }
}
template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare>::HeapQ(HeapQ &&other) noexcept {
    if (other.buffer_) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = other.buffer_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.buffer_ = nullptr;
    }
}
template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare> &HeapQ<_Tp, _Compare>::operator=(const HeapQ &other) {
    if (std::addressof(other) != this && other.buffer_) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = new ValueType[capacity_];
        std::copy(std::begin(other.buffer_), std::begin(other.buffer_) + size_,
                  std::begin(buffer_));
    }

    return *this;
}
template <typename _Tp, typename _Compare>
HeapQ<_Tp, _Compare> &HeapQ<_Tp, _Compare>::operator=(HeapQ &&other) noexcept {
    if (std::addressof(other) != this && other.buffer_) {
        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = other.buffer_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.buffer_ = nullptr;
    }
    return *this;
}
