#include <algorithm>
#include <iostream>
#include <memory>
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
        std::vector<w_node_t> go_{};

        Vertex() = default;
    };

    Vertex::shVert root_;
    AhoCorasick();
    void AddString(const std::string& s);
    Vertex::shVert GetLink(Vertex::shVert& v);
    Vertex::shVert Go(Vertex::shVert& v, char c);
};
AhoCorasick::AhoCorasick() {
    root_ = std::make_shared<Vertex>();
}
void AhoCorasick::AddString(const std::string& s) {
    Vertex::shVert v = root_;
    for (auto& c : s) {
        auto it =
            std::find_if(v->children_.begin(), v->children_.end(),
                         [c](Vertex::node_t& val) { return val.first == c; });

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

AhoCorasick::Vertex::shVert AhoCorasick::GetLink(
    AhoCorasick::Vertex::shVert& v) {
    if (v->link_.expired()) {
        if (v == root_ || v->parent_.lock() == root_) {
            v->link_ = root_;
        } else {
            auto tmp_v = v->parent_.lock();    // GetLink()
            tmp_v = GetLink(tmp_v);
            v->link_ = Go(tmp_v, v->par_cg_);
        }
    }
    return v->link_.lock();
}

AhoCorasick::Vertex::shVert AhoCorasick::Go(AhoCorasick::Vertex::shVert& v,
                                            char c) {
    auto it =
        std::find_if(v->go_.begin(), v->go_.end(),
                     [c](Vertex::w_node_t& val) { return val.first == c; });

    if (it == v->go_.end()) {

        auto it_next =
            std::find_if(v->children_.begin(), v->children_.end(),
                         [c](Vertex::node_t& val) { return val.first == c; });

        if (it_next != v->children_.end()) {
            v->go_.emplace_back(*it_next);
        } else {
            if (v == root_) {
                v->go_.emplace_back(c, root_);
            } else {
                auto tmp_v = GetLink(v);
                tmp_v = Go(tmp_v, c);
                v->go_.emplace_back(c, tmp_v);
            }
        }

        return v->go_.back().second.lock();
    } else {
        return it->second.lock();
    }
}

void PrintPat(AhoCorasick::Vertex::shVert q,
              const AhoCorasick::Vertex::shVert root, char c) {
    std::string out;
    out += c;

    while (q != root) {
        out += q->par_cg_;
        q = q->parent_.lock();
    }
    std::reverse(out.begin(), out.end());
    std::cout << out << "\n";
}

int main() {
    AhoCorasick ak;
    ak.AddString("abae");
    ak.AddString("qwerty");

    std::string s = "abae??abae??asdqqqwertyabae";

    AhoCorasick::Vertex::shVert q = ak.root_;

    for (char c : s) {
        while (q != ak.root_ && ak.Go(q, c) == ak.root_) {
            q = ak.GetLink(q);
        }
        q = ak.Go(q, c);
        if (q->leaf_) {
            PrintPat(q, ak.root_, c);
        }
    }
}
