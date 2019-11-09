// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <iostream>
#include <queue>
#include <vector>
#include "heap_q.h"

using std::pair;
using std::priority_queue;
using std::vector;
using pint = pair<int, int>;
using std::istringstream;
using std::ostringstream;

void Solutiuon(std::istream &iss, std::ostream &oss) {
    int n;
    int num_tup = 1;
    priority_queue<int, vector<int>, std::greater<>> p_queue;
    pint cur_time;

    iss >> n;
    iss >> cur_time.first >> cur_time.second;
    p_queue.emplace(cur_time.second);

    for (int i = 1; i < n; ++i) {
        iss >> cur_time.first >> cur_time.second;

        if (cur_time.first > p_queue.top()) {
            auto top_el = p_queue.top();
            p_queue.pop();
            p_queue.emplace(cur_time.second);
        } else {
            num_tup++;
            p_queue.emplace(cur_time.second);
        }
    }
    oss << num_tup;
}

void Solutiuon2(std::istream &iss, std::ostream &oss) {
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

int main() {
    Solutiuon(std::cin, std::cout);

    return 0;
}
