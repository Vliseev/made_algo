#include <algorithm>
#include "count_inverse.h"
#include "test_runner.h"

void TestMerge() {
    Vint v = {69, 84, 33, 200};
    Vint tmp(v.size());

    Merge(v.begin(), v.begin() + 2, v.begin() + 2, v.end(), tmp.begin());
    ASSERT_EQUAL(tmp, Vint({33, 69, 84, 200}))
}
void TestSort1() {
    Vint v = {69, 84, 33, 200};
    Vint ans = v;
    std::sort(ans.begin(), ans.end());

    MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(v, ans)
}
void TestSort2() {
    Vint v = {69, 84, 33, 200, 4};
    Vint ans = v;
    std::sort(ans.begin(), ans.end());

    MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(v, ans)
}
void TestSort3() {
    Vint v = {84, 69};
    Vint ans = v;
    std::sort(ans.begin(), ans.end());

    MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(v, ans)
}
void SortTest() {
    TestRunner tr;
    RUN_TEST(tr, TestMerge);
    RUN_TEST(tr, TestSort1);
    RUN_TEST(tr, TestSort2);
    RUN_TEST(tr, TestSort3);
}

void CounTestBase() {
    Vint v = {1, 2, 3, 4};

    size_t corr_ans = 0;
    size_t ans = MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(ans, corr_ans)

    v = {4, 3, 2, 1};
    corr_ans = 6;
    ans = MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(ans, corr_ans)

    v = {3, 2, 2};
    corr_ans = 2;
    ans = MergeSort(v.begin(), v.end());
    ASSERT_EQUAL(ans, corr_ans)
}

void CountTest() {
    TestRunner tr;
    RUN_TEST(tr, CounTestBase);
}

int main() {
    SortTest();
    CountTest();
    return 0;
}
