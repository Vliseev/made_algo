#include <iostream>
#include <sstream>
#include "Tree.h"
#include "test_runner.h"

void Solution(std::istream& iss, std::ostream& oss) {
    int n;
    Tree<int> t;

    iss >> n;
    for (int kI = 0; kI < n; ++kI) {
        int val;
        iss >> val;
        t.Insert(val);
    }
    t.PrintInorder(oss);
}

void Test1() {
    std::istringstream iss(
        "10\n"
        "9\n"
        "10\n"
        "4\n"
        "3\n"
        "2\n"
        "7\n"
        "8\n"
        "5\n"
        "1\n"
        "6");
    std::ostringstream oss;
    Solution(iss, oss);

    ASSERT_EQUAL(oss.str(), "1 2 3 4 5 6 7 8 9 10 ");

    oss.str("");
    oss.clear();
    iss.clear();
    iss.str(
        "10\n"
        "4\n"
        "7\n"
        "5\n"
        "10\n"
        "3\n"
        "2\n"
        "8\n"
        "1\n"
        "6\n"
        "9");
    Solution(iss, oss);
    ASSERT_EQUAL(oss.str(), "1 2 3 4 5 6 7 8 9 10 ");

    oss.str("");
    oss.clear();
    iss.clear();
    iss.str(
        "10\n"
        "2\n"
        "3\n"
        "6\n"
        "1\n"
        "10\n"
        "8\n"
        "4\n"
        "9\n"
        "7\n"
        "5");
    Solution(iss, oss);
    ASSERT_EQUAL(oss.str(), "1 2 3 4 5 6 7 8 9 10 ");
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test1);
    return 0;
}
