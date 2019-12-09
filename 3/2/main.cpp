#include <iostream>
#include <stack>
#include <string>
#include <vector>

template <class T, typename Compare = std::less<T>>
class SplayTree {
   public:
    SplayTree() = default;

    explicit SplayTree(const T& val);
    SplayTree(const SplayTree&) = delete;
    SplayTree(SplayTree&&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;
    SplayTree& operator&&(SplayTree&&) = delete;
    ~SplayTree();

    void Insert(const T& value);

    bool Find(const T& val) const;
    void Remove(const T& val);
    T KthNode(size_t k);

   private:
    struct Node {
        explicit Node(const T& value) : val_(value) {
        }
        T val_{};
        Node* left_ = nullptr;
        Node* right_ = nullptr;
        Node* parent_ = nullptr;
        size_t size_ = 1;
    };

    Node* root_ = nullptr;
    Compare cmp_ = Compare();
    static void DeleteSubtree(Node* node);

    void Splay(Node* pivot);
    void RightTurn(Node* parent);

    void LeftTurn(Node* parent);
    void UpdateSize(Node* node);
    T KthNode(Node* node, size_t k);

    SplayTree::Node* Merge(Node* t1, Node* t2);
};

template <class T, typename Compare>
void SplayTree<T, Compare>::RightTurn(Node* parent) {
    SplayTree::Node* node = parent->left_;
    if (node == nullptr)
        return;
    else {
        parent->left_ = node->right_;
        if (node->right_)
            node->right_->parent_ = parent;
        node->parent_ = parent->parent_;
        if (parent->parent_ == nullptr)
            root_ = node;
        else if (parent == node->parent_->left_)
            node->parent_->left_ = node;
        else
            parent->parent_->right_ = node;
        node->right_ = parent;
        parent->parent_ = node;

        UpdateSize(parent);
        UpdateSize(node);
    }
}

template <class T, typename Compare>
void SplayTree<T, Compare>::LeftTurn(Node* parent) {
    Node* node = parent->right_;
    if (node == nullptr)
        return;
    else {
        parent->right_ = node->left_;
        if (node->left_)
            node->left_->parent_ = parent;
        node->parent_ = parent->parent_;
        if (parent->parent_ == nullptr)
            root_ = node;
        else if (parent == parent->parent_->left_)
            parent->parent_->left_ = node;
        else
            parent->parent_->right_ = node;
        node->left_ = parent;
        parent->parent_ = node;
        UpdateSize(parent);
        UpdateSize(node);
    }
}

template <class T, typename Compare>
void SplayTree<T, Compare>::Splay(Node* pivot) {
    while (pivot->parent_ != nullptr) {
        if (pivot == pivot->parent_->left_) {
            if (pivot->parent_->parent_ == nullptr) {
                RightTurn(pivot->parent_);
            } else if (pivot->parent_ == pivot->parent_->parent_->left_) {
                RightTurn(pivot->parent_->parent_);
                RightTurn(pivot->parent_);
            } else {
                RightTurn(pivot->parent_);
                LeftTurn(pivot->parent_);
            }
        } else {
            if (pivot->parent_->parent_ == nullptr) {
                LeftTurn(pivot->parent_);
            } else if (pivot->parent_ == pivot->parent_->parent_->right_) {
                LeftTurn(pivot->parent_->parent_);
                LeftTurn(pivot->parent_);
            } else {
                LeftTurn(pivot->parent_);
                RightTurn(pivot->parent_);
            }
        }
    }
}

template <class T, typename Compare>
bool SplayTree<T, Compare>::Find(const T& val) const {
    if (root_ == nullptr)
        return false;
    Node* p = root_;
    Node* q = root_;

    while (p != nullptr && p->val_ != val) {
        if (cmp_(val, p->val_)) {
            q = p;
            p = p->left_;
        } else {
            q = p;
            p = p->right_;
        }
    }

    if (p != nullptr) {
        Splay(p);
        return true;
    } else {
        Splay(q);
        return false;
    }
}

template <class T, typename Compare>
void SplayTree<T, Compare>::Insert(const T& value) {
    if (root_ == nullptr)
        root_ = new Node(value);
    else {
        Node* p = root_;
        for (Node* q = p; q != nullptr; p = q ? q : p) {
            ++(q->size_);
            if (cmp_(value, q->val_))
                q = q->left_;
            else
                q = q->right_;
        }

        if (cmp_(value, p->val_)) {
            p->left_ = new Node(value);
            p->left_->parent_ = p;
            Splay(p->left_);
            return;
        } else {
            p->right_ = new Node(value);
            p->right_->parent_ = p;
            Splay(p->right_);
            return;
        }
    }
}

template <class T, typename Compare>
typename SplayTree<T, Compare>::Node* SplayTree<T, Compare>::Merge(Node* t1,
                                                                   Node* t2) {
    Node* p_i = t1;
    if (p_i == nullptr)
        return t2;
    while (p_i->right_) {
        p_i = p_i->right_;
    }
    Splay(p_i);
    p_i->right_ = t2;
    if (t2)
        t2->parent_ = p_i;

    UpdateSize(p_i);
    return p_i;
}

template <class T, typename Compare>
void SplayTree<T, Compare>::DeleteSubtree(Node* node) {
    std::stack<Node*> s;
    s.push(node);

    while (!s.empty()) {
        auto tmp_node = s.top();
        s.pop();
        if (tmp_node->left_)
            s.push(tmp_node->left_);
        if (tmp_node->right_)
            s.push(tmp_node->right_);
        delete tmp_node;
    }
}

template <class T, typename Compare>
SplayTree<T, Compare>::~SplayTree() {
    DeleteSubtree(root_);
}

template <class T, typename Compare>
void SplayTree<T, Compare>::Remove(const T& val) {
    if (root_ == nullptr)
        return;
    Node* p = root_;

    while (p != nullptr && p->val_ != val) {
        if (cmp_(val, p->val_))
            p = p->left_;
        else
            p = p->right_;
    }

    if (p != nullptr) {
        Splay(p);
        root_ = Merge(p->left_, p->right_);
        root_->parent_ = nullptr;
        delete p;
        return;
    } else {
        return;
    }
}

template <class T, typename Compare>
SplayTree<T, Compare>::SplayTree(const T& val)
    : root_(new Node(val)), cmp_(Compare()) {
}

template <class T, typename Compare>
void SplayTree<T, Compare>::UpdateSize(Node* node) {
    node->size_ = 1;
    if (node->left_)
        node->size_ += node->left_->size_;
    if (node->right_)
        node->size_ += node->right_->size_;
}

template <class T, typename Compare>
T SplayTree<T, Compare>::KthNode(size_t k) {
    if (k > root_->size_)
        throw std::logic_error("invalid size");
    return KthNode(root_, k);
}

template <class T, typename Compare>
T SplayTree<T, Compare>::KthNode(Node* node, size_t k) {
    size_t r = node->left_ ? node->left_->size_ : 0;
    if (k == r) {
        int val = node->val_;
        Splay(node);
        return val;
    } else if (cmp_(k, r))
        return KthNode(node->left_, k);
    else
        return KthNode(node->right_, k - (r + 1));
}

using SpTc = SplayTree<int>;
void Solution(std::istream& iss) {
    int n;
    SpTc sp;

    iss >> n;
    for (int kI = 0; kI < n; ++kI) {
        int val;
        int k;
        iss >> val >> k;
        if (val > 0)
            sp.Insert(val);
        else
            sp.Remove(-val);
        std::cout << sp.KthNode(k) << " ";
    }
}

int main() {
    Solution(std::cin);

    return 0;
}