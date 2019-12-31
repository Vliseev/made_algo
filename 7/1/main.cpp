#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

using st_type = std::array<uint8_t, 16>;

constexpr st_type FState = {1, 2,  3,  4,  5,  6,  7,  8,
                            9, 10, 11, 12, 13, 14, 15, 0};

struct State {
    static constexpr std::pair<int, int> shift_pos_[4] = {
        {-1, 0}, {0, -1}, {0, 1}, {1, 0}};

    st_type a_ = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
    uint8_t zero_pos_ = 0;
    //    std::vector<char> moves_{};
    size_t h_{};
    size_t d_{};

    explicit State(const st_type& other);

    State(const State& other) = default;
    State(State&& other) noexcept = default;
    State& operator=(const State& other) = default;
    State& operator=(State&& other) noexcept = default;

    bool operator==(const State& other) const;
    bool operator!=(const State& other) const {
        return !operator==(other);
    }

    std::vector<State> GetChildren() const;
    size_t ComputeHeuristic() const;
    std::pair<int, int> GetPos(int val) const;
};

struct StateEq {
    bool operator()(const State& s_1, const State& s_2) const {
        return std::equal(std::begin(s_1.a_), std::end(s_1.a_),
                          std::begin(s_2.a_));
    }
};

struct StateHash {
    size_t operator()(const State& s1) const {
        return std::_Hash_impl::hash(s1.a_);
    }
};

using p_unmap_t = std::unordered_map<State, State, StateHash, StateEq>;

char GetMoveSymbol(const State& from, const State& to) {
    char zero_diff = to.zero_pos_ - from.zero_pos_;
    switch (zero_diff) {
        case 1:
            return 'L';
        case -1:
            return 'R';
        case 4:
            return 'U';
        case -4:
            return 'D';
        default:
            assert(false);
    }
    return 0;
}

std::vector<char> GetPath(const State& start, const p_unmap_t& parents) {
    std::vector<char> result;

    State current = State(FState);
    while (current != start) {
        const State parent = parents.at(current);
        result.push_back(GetMoveSymbol(parent, current));
        current = parent;
    }
    // Развернем result, так как собирали его с конца.
    std::reverse(result.begin(), result.end());
    return result;
}

std::vector<State> State::GetChildren() const {
    std::vector<State> children;

    for (auto [i, j] : shift_pos_) {
        int new_i = zero_pos_ / 4 + i;
        int new_j = zero_pos_ % 4 + j;
        if (new_i >= 0 && new_i <= 3 && new_j >= 0 && new_j <= 3) {
            auto c_state = *this;
            std::swap(c_state.a_[zero_pos_], c_state.a_[new_i * 4 + new_j]);

            c_state.zero_pos_ = new_i * 4 + new_j;
            c_state.h_ = c_state.ComputeHeuristic();
            c_state.d_ += 1;

            children.push_back(c_state);
        }
    }
    return children;
}

State::State(const st_type& other) {
    std::copy(std::begin(other), std::end(other), std::begin(a_));
    for (int i = 0; i < 16; ++i) {
        if (a_[i] == 0) {
            zero_pos_ = i;
            break;
        }
    }
}

size_t State::ComputeHeuristic() const {
    size_t heuristic = 0;
    for (int val = 1; val < 16; ++val) {
        int i = (val - 1) / 4;
        int j = (val - 1) % 4;
        auto [p_i, p_j] = GetPos(val);
        heuristic += std::abs(i - p_i) + std::abs(j - p_j);
    }

    return heuristic;
}

std::pair<int, int> State::GetPos(int val) const {
    for (int i = 0; i < 16; ++i) {
        if (a_[i] == val) {
            return {i / 4, i % 4};
        }
    }
    return {-1, -1};
}
bool State::operator==(const State& other) const {
    return a_ == other.a_;
}

std::vector<char> GetSolution(State& s) {
    std::vector<char> steps;
    bool can_stop = false;

    auto lambda_cmp = [](const State& s1, const State& s2) {
        return s1.h_ + s1.d_ / 1.7 > s2.h_ + s2.d_ / 1.7;
    };

    auto hash_f = [](const st_type& s1) { return std::_Hash_impl::hash(s1); };
    auto eq_f = [](const st_type& s1, const st_type& s2) {
        return std::equal(std::begin(s1), std::end(s1), std::begin(s2));
    };

    std::priority_queue<State, std::vector<State>, decltype(lambda_cmp)>
        p_queue(lambda_cmp);

    p_unmap_t parent_map;

    s.h_ = s.ComputeHeuristic();
    p_queue.push(s);

    while (!p_queue.empty() && !can_stop) {
        auto cur_state = p_queue.top();
        p_queue.pop();

        for (auto&& child : cur_state.GetChildren()) {
            if (parent_map.count(child))
                continue;

            parent_map.insert({child, cur_state});
            if (child.h_ == 0) {
                steps = GetPath(s, parent_map);
                can_stop = true;
                break;
            }

            p_queue.push(child);
        }
    }
    return steps;
}

void ReadField(std::istream& iss, st_type& desk) {
    for (auto& de : desk) {
        int val;
        iss >> val;
        de = val;
    }
}

int NumInvers(const st_type& desk) {
    int num_inv = 0;
    for (int i = 0; i < 15; ++i) {
        for (int j = i + 1; j < 16; j++) {
            if (desk[i] && desk[j] && desk[i] > desk[j])
                num_inv++;
        }
    }
    return num_inv;
}

bool IsSolvable(const State& s) {
    int num_inv = NumInvers(s.a_);
    auto [y, x] = s.GetPos(0);
    return y & 1 ? !(num_inv & 1) : num_inv & 1;
}

int main() {
    st_type desk;

    ReadField(std::cin, desk);
    State s(desk);

    if (!IsSolvable(s)) {
        std::cout << -1;
        return 0;
    }

    auto sol = GetSolution(s);

    std::cout << sol.size() << "\n";
    for (auto s : sol) {
        std::cout << s;
    }

    return 0;
}
