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
        using shVert = std::shared_ptr<Vertex>;
        using weakVert = std::weak_ptr<Vertex>;

        using node_t = std::pair<char, shVert>;
        using w_node_t = std::pair<char, weakVert>;

        std::vector<node_t> children_{};
        bool leaf_ = false;
        weakVert parent_{};
        char par_cg_{};
        weakVert link_{};

        Vertex() = default;

        std::vector<node_t>::iterator FindKey(char c);
        bool Count(char c);
    };
    using shVert = Vertex::shVert;
    using weakVert = Vertex::weakVert;

    shVert root_;
    AhoCorasick();
    void AddString(std::string_view s);
    void InitFSM();
    shVert Go(const shVert& v, char c);
    shVert GetLink(const shVert& v);
};
std::vector<AhoCorasick::Vertex::node_t>::iterator AhoCorasick::Vertex::FindKey(
    char c) {
    return std::find_if(children_.begin(), children_.end(),
                        [c](Vertex::node_t& val) { return val.first == c; });
}
bool AhoCorasick::Vertex::Count(char c) {
    return std::find_if(children_.begin(), children_.end(),
                        [c](Vertex::node_t& val) { return val.first == c; }) !=
           children_.end();
}
AhoCorasick::AhoCorasick() {
    root_ = std::make_shared<Vertex>();
}
void AhoCorasick::AddString(std::string_view s) {
    Vertex::shVert v = root_;
    for (auto& c : s) {
        auto it = v->FindKey(c);
        if (it == v->children_.end()) {
            auto child = std::make_shared<Vertex>();
            child->parent_ = v;
            child->par_cg_ = c;
            v->children_.emplace_back(c, std::move(child));
            v = v->children_.back().second;
        } else {
            v = it->second;
        }
    }
    v->leaf_ = true;
}
void AhoCorasick::InitFSM() {
    std::queue<Vertex::shVert> queue;

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
AhoCorasick::shVert AhoCorasick::Go(const AhoCorasick::shVert& v, char c) {
    auto it = v->FindKey(c);
    return it != v->children_.end() ? it->second : root_;
}
AhoCorasick::shVert AhoCorasick::GetLink(const AhoCorasick::shVert& v) {
    return v->link_.lock();
}

void PrintPat(AhoCorasick::shVert q, const AhoCorasick::shVert& root) {
    std::string out;

    while (q != root) {
        out += q->par_cg_;
        q = q->parent_.lock();
    }
    std::reverse(out.begin(), out.end());
    std::cout << out << "\n";
}

void Out(AhoCorasick::shVert q, const AhoCorasick::shVert& root) {
    auto tmp_ptr = std::move(q);
    while (tmp_ptr && tmp_ptr != root) {
        if (tmp_ptr->leaf_)
            PrintPat(tmp_ptr, root);
        tmp_ptr = tmp_ptr->link_.lock();
    }
}

std::pair<std::string_view, std::optional<std::string_view>> SplitTwoStrict(
    std::string_view s, char delimiter) {
    size_t pos = s.find(delimiter);
    if (pos == s.npos) {
        return {s, std::nullopt};
    } else {
        size_t num_del = 1;
        while (pos + num_del < s.npos && s[pos + num_del] == delimiter)
            num_del++;
        return {s.substr(0, pos), s.substr(pos + num_del)};
    }
}

std::vector<std::string_view> Split(std::string_view s, char delimiter) {
    std::vector<std::string_view> parts;
    size_t cur_pos = 0, beg_pos;
    if (s.empty()) {
        return parts;
    }

    while (cur_pos < s.size() && s[cur_pos] == delimiter)
        cur_pos++;
    beg_pos = cur_pos;

    while (cur_pos < s.size()) {
        if (s[cur_pos] == delimiter) {
            parts.push_back(s.substr(beg_pos, cur_pos - beg_pos));
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

int main() {
    AhoCorasick ak;
    std::string pat = "?a?qwe???teq??sad???aw?";
    auto tok = Split(pat, '?');

    for (auto v : tok) {
        std::cout << v << "\n";
//        ak.AddString(v);
    }

//    ak.InitFSM();
    //
    //    std::string s = "abae??abaek??asdqqqwertyabae";
    //    //    std::string s = "qqwertyabae";
    //
    //    AhoCorasick::shVert q = ak.root_;
    //
    //    for (char c : s) {
    //        while (q != ak.root_ && ak.Go(q, c) == ak.root_) {
    //            q = ak.GetLink(q);
    //        }
    //        q = ak.Go(q, c);
    //        Out(q, ak.root_);
    //    }
    return 0;
}
