#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include "count_inverse.h"

void Solution(std::istream &iss) {
    std::vector<int> v;
    v.reserve(10000);
    int tmp;
    while (iss >> tmp) {
        v.push_back(tmp);
    }
    std::cout << MergeSort(v.begin(), v.end());
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Solution(std::cin);
    return 0;
}
