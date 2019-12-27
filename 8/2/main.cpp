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
        uint32_t leaf_ = std::numeric_limits<uint32_t>::max();
        WeakVert link_{};

        Vertex() = default;

        std::vector<NodeT>::iterator FindKey(char c);
        bool Count(char c);
    };
    using ShVert = Vertex::ShVert;
    using WeakVert = Vertex::WeakVert;
    using PatternT = std::pair<size_t, std::string>;

    ShVert root_;
    std::vector<PatternT> patterns_{};

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
void AhoCorasick::AddString(std::string_view s, size_t pos) {
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
    patterns_.emplace_back(pos, s);
    v->leaf_ = patterns_.size() - 1;
}
void AhoCorasick::InitFSM() {
    std::queue<Vertex::ShVert> queue;

    for (const auto& v : root_->children_) {
        queue.push(v.second);
        v.second->link_ = root_;
    }

    while (!queue.empty()) {
        auto rnode = queue.front();
        queue.pop();

        for (auto [k, node] : rnode->children_) {
            queue.push(node);
            auto fnode = rnode->link_.lock();
            while (fnode && !fnode->Count(k)) {
                fnode = fnode->link_.lock();
            }
            if (fnode) {
                node->link_ = fnode->FindKey(k)->second;
            } else {
                node->link_ = root_;
            }
        }
    }
}
AhoCorasick::ShVert AhoCorasick::Go(const AhoCorasick::ShVert& v, char c) {
    auto it = v->FindKey(c);
    return it != v->children_.end() ? it->second : root_;
}
AhoCorasick::ShVert AhoCorasick::GetLink(const AhoCorasick::ShVert& v) {
    return v->link_.lock();
}

void Out(AhoCorasick::ShVert q, const AhoCorasick& ak, std::vector<int>& C,
         int j) {
    auto tmp_ptr = std::move(q);
    while (tmp_ptr && tmp_ptr != ak.root_) {
        if (tmp_ptr->leaf_ != std::numeric_limits<uint32_t>::max())
            C[j - ak.patterns_[tmp_ptr->leaf_].first -
              ak.patterns_[tmp_ptr->leaf_].second.size()+1]++;
        tmp_ptr = tmp_ptr->link_.lock();
    }
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
            parts.emplace_back(beg_pos, s.substr(beg_pos, cur_pos - beg_pos));
            while (cur_pos < s.size() && s[cur_pos] == delimiter) {
                cur_pos++;
            }
            beg_pos = cur_pos;
        } else {
            cur_pos++;
        }
    }
    return parts;
}

void Solution(AhoCorasick& ak, std::string_view s) {
    auto q = ak.root_;
    std::vector<int> C(s.size(), 0);

    int j = 0;
    for (char c : s) {
        while (q != ak.root_ && ak.Go(q, c) == ak.root_) {
            q = ak.GetLink(q);
        }
        q = ak.Go(q, c);
        Out(q, ak, C, j);
        j++;
    }

    auto num_pat = ak.patterns_.size();
    for (int i = 0; i < C.size(); i++) {
        if (C[i] == num_pat)
            std::cout << i << "\n";
    }
}

AhoCorasick BuildAk(std::string_view pat) {
    AhoCorasick ak;
    auto tok = Split(pat, '?');

    for (auto& [idx, v] : tok) {
        ak.AddString(v, idx);
    }
    ak.InitFSM();

    return ak;
}

int main() {
    AhoCorasick ak;
    std::string pat = "aa?aa?";
    std::string_view s = "eaakaaaab";

    //    std::string pat = "ab??ab";
//    std::string_view s = "xabvccababcax";

    ak = BuildAk(pat);
    Solution(ak, s);

    return 0;
}
