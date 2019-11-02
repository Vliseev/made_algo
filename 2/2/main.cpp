#include <algorithm>
#include <iterator>
#include <vector>
#include "part.h"

void Solution(std::istream &iss) {
    int n, p;
    std::vector<int> v;

    std::cin >> n >> p;
    v.reserve(n);
    std::copy(std::istream_iterator<int>(iss), std::istream_iterator<int>(),
              std::back_inserter(v));

    Select(std::begin(v), std::end(v), p);
    std::cout << v[p];
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Solution(std::cin);
}
