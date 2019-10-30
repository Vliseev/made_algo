#include <iostream>
#include <sstream>
#include <vector>
#include "CircularQueue.h"
#include "test_runner.h"

enum Command { POP_FRONT = 2, PUSH_BACK = 3 };

void Solution(std::istream &iss, std::ostream &oss) {
  int n, val, out_val;
  Command c;
  CircularQueue<int> queue;

  iss >> n;

  for (int i = 0; i < n; ++i) {
    iss >> val;
    c = static_cast<Command>(val);
    iss >> val;
    switch (c) {
      case PUSH_BACK:
        queue.PushBack(val);
        break;
      case POP_FRONT:
        auto flag = queue.TryPop(out_val);
        if (flag != -1) {
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

void Test3() {
  using std::istringstream;
  using std::ostringstream;

  ostringstream oss{};
  istringstream iss(
      "16\n"
      "2 -1\n"
      "2 -1\n"
      "3 1\n"
      "3 15\n"
      "3 11\n"
      "2 1\n"
      "2 15\n"
      "3 8\n"
      "2 11\n"
      "3 16\n"
      "3 1\n"
      "3 16\n"
      "2 8\n"
      "2 16\n"
      "2 1\n"
      "2 16");
  Solution(iss, oss);
  ASSERT_EQUAL("YES", oss.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test1);
  RUN_TEST(tr, Test2);
  RUN_TEST(tr, Test3);
  return 0;
}
