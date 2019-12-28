#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <utility>

class AhoCorasick {
   public:
    struct Vertex {
        using ShVert = std::shared_ptr<Vertex>;
        using WeakVert = std::weak_ptr<Vertex>;

        using NodeT = std::pair<char, ShVert>;
        using WNodeT = std::pair<char, WeakVert>;

        std::vector<NodeT> children_{};
        std::vector<int> out_l_{};
        WeakVert f_{};

        Vertex() = default;

        std::vector<NodeT>::iterator FindKey(char c);
        bool Count(char c);
    };
    using ShVert = Vertex::ShVert;
    using WeakVert = Vertex::WeakVert;


    AhoCorasick();
    AhoCorasick(AhoCorasick&& other) noexcept = default;
    AhoCorasick& operator=(AhoCorasick&& other) noexcept = default;
    AhoCorasick(const AhoCorasick&) = default;
    AhoCorasick& operator=(const AhoCorasick&) = default;
    ~AhoCorasick() = default;

    void AddString(std::string_view s, size_t);
    void InitFSM();
    ShVert Go(const ShVert& v, char c);
    ShVert GetLink(const ShVert& v);

   private:
    ShVert root_;

   public:
    [[nodiscard]] const ShVert& GetRoot() const {
        return root_;
    }
    [[nodiscard]] int GetNumPat() const {
        return num_pat;
    }

   private:
    int num_pat{};
};
std::vector<AhoCorasick::Vertex::NodeT>::iterator
AhoCorasick::Vertex::FindKey(char c) {
    return std::find_if(children_.begin(), children_.end(),
                        [c](Vertex::NodeT& val) { return val.first == c; });
}
bool AhoCorasick::Vertex::Count(char c) {
    return std::find_if(children_.begin(), children_.end(),
                        [c](Vertex::NodeT& val) { return val.first == c; }) !=
           children_.end();
}
AhoCorasick::AhoCorasick() {
    root_ = std::make_shared<Vertex>();
}
void AhoCorasick::AddString(std::string_view s, size_t l) {
    Vertex::ShVert v = root_;
    for (auto& c : s) {
        auto it = v->FindKey(c);
        if (it == v->children_.end()) {
            auto child = std::make_shared<Vertex>();
            v->children_.emplace_back(c, std::move(child));
            v = v->children_.back().second;
        } else {
            v = it->second;
        }
    }
    v->out_l_.push_back(l);
    num_pat++;
}
void AhoCorasick::InitFSM() {
    std::queue<Vertex::ShVert> queue;

    for (const auto& v : root_->children_) {
        queue.push(v.second);
        v.second->f_ = root_;
    }

    while (!queue.empty()) {
        auto rnode = queue.front();
        queue.pop();

        for (auto [k, node] : rnode->children_) {
            queue.push(node);
            auto fnode = rnode->f_.lock();
            while (fnode && !fnode->Count(k)) {
                fnode = fnode->f_.lock();
            }
            node->f_ = fnode ? fnode->FindKey(k)->second : root_;

            auto cpy_v = node->f_.lock()->out_l_;
            std::copy(cpy_v.begin(), cpy_v.end(),
                      std::back_inserter(node->out_l_));
        }
    }
}
AhoCorasick::ShVert AhoCorasick::Go(const AhoCorasick::ShVert& v, char c) {
    auto it = v->FindKey(c);
    return it != v->children_.end() ? it->second : root_;
}
AhoCorasick::ShVert AhoCorasick::GetLink(const AhoCorasick::ShVert& v) {
    return v->f_.lock();
}


std::vector<std::pair<size_t, std::string_view>> Split(std::string_view s,
                                                       char delimiter) {
    std::vector<std::pair<size_t, std::string_view>> parts;
    size_t cur_pos = 0, beg_pos;
    if (s.empty()) {
        return parts;
    }

    while (cur_pos < s.size() && s[cur_pos] == delimiter)
        cur_pos++;
    beg_pos = cur_pos;

    while (cur_pos < s.size()) {
        if (s[cur_pos] == delimiter) {
            parts.emplace_back(cur_pos, s.substr(beg_pos, cur_pos - beg_pos));
            while (cur_pos < s.size() && s[cur_pos] == delimiter) {
                cur_pos++;
            }
            beg_pos = cur_pos;
        } else {
            cur_pos++;
        }
    }
    if (beg_pos != cur_pos)
        parts.emplace_back(cur_pos, s.substr(beg_pos, cur_pos - beg_pos));
    return parts;
}

void Solution(AhoCorasick& ac, std::string_view s, std::string_view pat) {
    auto node = ac.GetRoot();
    std::vector<int> C(s.size(), 0);

    for (int i = 0; i < s.size(); i++) {
        while (node != ac.GetRoot() && !node->Count(s[i]))
            node = node->f_.lock();
        node = ac.Go(node, s[i]);

        for (auto l : node->out_l_)
            if (i - l + 1 >= 0)
                C[i - l + 1] += 1;
    }

    for (int i = 0; i < C.size(); i++) {
        if (C[i] == ac.GetNumPat() && i + pat.size() <= s.size())
            std::cout << i << "\n";
    }
}

AhoCorasick BuildAC(std::string_view pat, char delimeter) {
    AhoCorasick ac;
    auto tok = Split(pat, delimeter);

    for (auto& [idx, v] : tok) {
        ac.AddString(v, idx);
    }
    ac.InitFSM();

    return ac;
}

int main() {
    AhoCorasick ac;
    const char kDelimeter = '?';
    std::string pat;
    std::string s;

    std::cin >> pat >> s;

    ac = BuildAC(pat, kDelimeter);
    Solution(ac, s, pat);

    return 0;
}
