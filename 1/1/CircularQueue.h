#pragma once
#include <algorithm>
#include <utility>

/*1.1 Реализовать очередь с динамическим зацикленным буфером*/

template <class T>
class CircularQueue {
  using ValueType = T;

  size_t size_;
  size_t capacity_;
  ValueType *buffer_;
  size_t head_;
  size_t tail_;

  void Resize();

 public:
  explicit CircularQueue(size_t size = 4);
  void PushBack(const T &val);
  void PushBack(T &&val);
  bool TryPop(T &val);
  ~CircularQueue() {
    delete[] buffer_;
  }
  void Update();
};

template <class T>
CircularQueue<T>::CircularQueue(size_t size)
    : size_(0), capacity_(size), head_(0), tail_(0), buffer_(new T[capacity_]) {
}

template <class T>
void CircularQueue<T>::PushBack(const T &val) {
  buffer_[tail_] = val;
  Update();
}

template <class T>
void CircularQueue<T>::Update() {
  ++tail_;
  ++size_;
  tail_ %= capacity_;

  if (tail_ == head_)
    Resize();
}

template <class T>
void CircularQueue<T>::Resize() {
  ValueType *tmp_buf = new ValueType[capacity_ * 2];
  std::move(buffer_ + head_, buffer_ + capacity_, tmp_buf);
  std::move(buffer_, buffer_ + tail_, tmp_buf + capacity_ - head_);

  head_ = 0;
  tail_ = size_;

  capacity_ *= 2;
  delete[] buffer_;
  buffer_ = tmp_buf;
}
template <class T>
bool CircularQueue<T>::TryPop(T &val) {
  if (size_ == 0)
    return false;
  val = buffer_[head_];

  --size_;
  head_++;
  head_ %= capacity_;

  return true;
}
template <class T>
void CircularQueue<T>::PushBack(T &&val) {
  buffer_[tail_] = std::move(val);
  Update();
}
