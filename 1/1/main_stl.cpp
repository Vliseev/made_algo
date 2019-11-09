#include <iostream>
#include <queue>
#include <sstream>
#include <vector>
#include "test_runner.h"

enum Command { POP_FRONT = 2, PUSH_BACK = 3 };
//
// template<class T>
// class CircularQueue {
//  using ValueType = T;
//  using PointerType = T *;
//  using RefferenceType = T &;
//
//  size_t size_;
//  size_t capacity_;
//  PointerType buffer_;
//  size_t head_;
//  size_t tail_;
//
//  void Resize();
//
// public:
//  explicit CircularQueue(size_t size = 4);
//  void PushBack(const T &val);
//  void PushBack(T &&val);
//  int TryPop(T &val);
//  ~CircularQueue() { delete[] buffer_; }
//  void Update();
//};
//
// template<class T>
// CircularQueue<T>::CircularQueue(size_t size) : size_(0), capacity_(size) {
//  buffer_ = new T[capacity_];
//  head_ = 0;
//  tail_ = 0;
//}
//
// template<class T>
// void CircularQueue<T>::PushBack(const T &val) {
//  buffer_[tail_] = val;
//  Update();
//}
//
// template<class T>
// void CircularQueue<T>::Update() {
//  ++tail_;
//  ++size_;
//  tail_ %= capacity_;
//
//  if (tail_ == head_) Resize();
//}
//
// template<class T>
// void CircularQueue<T>::Resize() {
//  PointerType tmp_buf = new ValueType[capacity_ * 2];
//  std::copy(buffer_ + head_, buffer_ + capacity_, tmp_buf);
//  std::copy(buffer_, buffer_ + tail_, tmp_buf + capacity_);
//
//  head_ = 0;
//  tail_ = size_;
//
//  capacity_ *= 2;
//  delete[] buffer_;
//  buffer_ = tmp_buf;
//}
// template<class T>
// int CircularQueue<T>::TryPop(T &val) {
//  if (size_ == 0) return -1;
//  val = buffer_[head_];
//
//  --size_;
//  head_++;
//  head_ %= capacity_;
//
//  return 0;
//}
// template<class T>
// void CircularQueue<T>::PushBack(T &&val) {
//  buffer_[tail_] = std::move(val);
//  Update();
//}

void Solution(std::istream &iss, std::ostream &oss) {
  using std::queue;
  int n, val, out_val;
  Command c;
  queue<int> q;

  iss >> n;

  for (int i = 0; i < n; ++i) {
    iss >> val;
    c = static_cast<Command>(val);
    iss >> val;
    switch (c) {
      case PUSH_BACK:
        q.push(val);
        break;
      case POP_FRONT:
        if (!q.empty()) {
          out_val = q.front();
          q.pop();
          if (out_val != val) {
            oss << "NO";
            return;
          }
        } else {
          if (-1 != val) {
            oss << "NO";
            return;
          }
        }
        break;
    }
  }
  oss << "YES";
}

void Test1() {
  using std::cout;
  using std::istringstream;
  using std::ostringstream;

  ostringstream oss{};
  istringstream iss(
      "3\n"
      "3 44\n"
      "3 50\n"
      "2 44");
  Solution(iss, oss);
  ASSERT_EQUAL("YES", oss.str());

  iss.clear();
  oss.str("");
  oss.clear();
  iss.str(
      "3\n"
      "3 44\n"
      "3 50\n"
      "2 44");
  Solution(iss, oss);
  ASSERT_EQUAL("YES", oss.str());

  iss.clear();
  oss.str("");
  oss.clear();
  iss.str(
      "2\n"
      "3 44\n"
      "2 66");
  Solution(iss, oss);
  ASSERT_EQUAL(oss.str(), "NO");
}

void Test2() {
  using std::istringstream;
  using std::ostringstream;

  ostringstream oss{};
  istringstream iss(
      "3\n"
      "2 -1\n"
      "3 100\n"
      "2 100\n"
      "3 2\n"
      "3 4\n"
      "2 2");
  Solution(iss, oss);
  ASSERT_EQUAL("YES", oss.str());

  iss.clear();
  oss.str("");
  oss.clear();
  iss.str(
      "3\n"
      "3 44\n"
      "3 50\n"
      "2 44");
  Solution(iss, oss);
  ASSERT_EQUAL("YES", oss.str());
}

int main() {
  //    TestRunner tr;
  //    RUN_TEST(tr, Test1);
  //    RUN_TEST(tr, Test2);
  Solution(std::cin, std::cout);
  return 0;
}
