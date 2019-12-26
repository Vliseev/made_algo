#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <random>
#include <string>

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
    void AddString(const std::string& s);
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
void AhoCorasick::AddString(const std::string& s) {
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
    auto tmp_ptr = q;
    while (tmp_ptr && tmp_ptr != root) {
        if (tmp_ptr->leaf_)
            PrintPat(tmp_ptr, root);
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

    //    std::string s = "abae??abaek??asdqqqwertyabae";
    std::string s = "qqwertyabae";

    AhoCorasick::shVert q = ak.root_;

    for (char c : s) {
        while (q != ak.root_ && ak.Go(q, c) == ak.root_) {
            q = ak.GetLink(q);
        }
        q = ak.Go(q, c);
        Out(q, ak.root_);
    }
}
