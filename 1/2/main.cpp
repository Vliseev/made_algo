// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include "test_runner.h"
#include "heap_q.h"

using std::pair;
using std::vector;
using pint = pair<int, int>;
using std::istringstream;
using std::ostringstream;


void Solutiuon(std::istream &iss, std::ostream &oss) {
  int n;
  int num_tup = 1;
  HeapQ<int, std::greater<>> p_queue;
  pint cur_time;

  iss >> n;
  iss >> cur_time.first >> cur_time.second;
  p_queue.Add(cur_time.second);

  for (int i = 1; i < n; ++i) {
    iss >> cur_time.first >> cur_time.second;

    if (cur_time.first > p_queue.Top()) {
      auto top_el = p_queue.Top();
      p_queue.Pop();
      p_queue.Add(cur_time.second);
    } else {
      num_tup++;
      p_queue.Add(cur_time.second);
    }
  }
  oss << num_tup;
}

void Test1() {
  istringstream iss(
      "5\n"
      "2 4\n"
      "3 5\n"
      "5 7\n"
      "6 8\n"
      "7 9");
  ostringstream oss;

  Solutiuon(iss, oss);
  ASSERT_EQUAL(oss.str(), "3");
}

void Test2() {
  istringstream iss(
      "3\n"
      "10 20\n"
      "20 25\n"
      "21 30");
  ostringstream oss;

  Solutiuon(iss, oss);
  ASSERT_EQUAL(oss.str(), "2");

  iss.clear();
  iss.str(
      "2\n"
      "10 20\n"
      "20 25");
  oss.str("");
  oss.clear();
  Solutiuon(iss, oss);
  ASSERT_EQUAL(oss.str(), "2");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, Test1);
  RUN_TEST(tr, Test2);

  return 0;
}
