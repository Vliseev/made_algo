#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>

class AhoCorasick {
   public:
    struct Vertex {
        using ShVert = std::shared_ptr<Vertex>;
        using WeakVert = std::weak_ptr<Vertex>;

        using NodeT = std::pair<char, ShVert>;
        using WNodeT = std::pair<char, WeakVert>;

        std::vector<NodeT> children_{};
        uint32_t leaf_ = std::numeric_limits<uint32_t>::max();
        char par_cg_{};
        WeakVert link_{};

        Vertex() = default;

        std::vector<NodeT>::iterator FindKey(char c);
        bool Count(char c);
    };
    using ShVert = Vertex::ShVert;
    using WeakVert = Vertex::WeakVert;

    ShVert root_;
    std::vector<std::string> strings{};

    AhoCorasick();
    void AddString(const std::string& s);
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
void AhoCorasick::AddString(const std::string& s) {
    Vertex::ShVert v = root_;
    for (auto& c : s) {
        auto it = v->FindKey(c);
        if (it == v->children_.end()) {
            auto child = std::make_shared<Vertex>();
            child->par_cg_ = c;
            v->children_.emplace_back(c, std::move(child));
            v = v->children_.back().second;
        } else {
            v = it->second;
        }
    }
    strings.push_back(s);
    v->leaf_ = strings.size() - 1;
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

void Out(AhoCorasick::ShVert q, const AhoCorasick& ak) {
    auto tmp_ptr = q;
    while (tmp_ptr && tmp_ptr != ak.root_) {
        if (tmp_ptr->leaf_ != std::numeric_limits<uint32_t>::max())
            std::cout << ak.strings[tmp_ptr->leaf_] << "\n";
        tmp_ptr = tmp_ptr->link_.lock();
    }
}

int main() {
    AhoCorasick ak;
    ak.AddString("abae");
    ak.AddString("aek");
    ak.AddString("qwerty");
    ak.AddString("wer");
    ak.InitFSM();

    std::string s = "abae??abaek??asdqqqwertyabae";
    //    std::string s = "qqwertyabae";

    AhoCorasick::ShVert q = ak.root_;

    for (char c : s) {
        while (q != ak.root_ && ak.Go(q, c) == ak.root_) {
            q = ak.GetLink(q);
        }
        q = ak.Go(q, c);
        Out(q, ak);
    }
}
