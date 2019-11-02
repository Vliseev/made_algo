#include "part.h"
#include "test_runner.h"

void Test() {
    std::vector<int> v = {2, 8, 7, 1, 4, 3, 5, 6};
    int ans, corr_ans;
    for (int kI = 0; kI < v.size(); ++kI) {
        Select(std::begin(v), std::end(v), kI);
        ans = v[kI];
        std::nth_element(std::begin(v), std::end(v), std::begin(v) + kI);
        corr_ans = v[kI];
        ASSERT_EQUAL(ans, corr_ans);
    }

    v = {2, 8, 7, 1, 4, 3, 5, 6, 19};
    for (int kI = 0; kI < v.size(); ++kI) {
        Select(std::begin(v), std::end(v), kI);
        ans = v[kI];
        std::nth_element(std::begin(v), std::end(v), std::begin(v) + kI);
        corr_ans = v[kI];
        ASSERT_EQUAL(ans, corr_ans);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);

    return 0;
}
