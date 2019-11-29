//
// Created by user on 05.11.2019.
//

#pragma once
#include <iostream>
#include <stack>

template <class T, class Less = std::less<T>>
class Tree {
   public:
    Tree() = default;
    explicit Tree(const T& val);

    Tree(const Tree& other) = delete;
    Tree(Tree&& other) = delete;

    Tree& operator=(const Tree& other) = delete;
    Tree& operator=(Tree&& other) = delete;

    ~Tree();

    template <class UnaryFunction>
    void InordTraversal(UnaryFunction f);
    void Insert(const T& value);

   private:
    struct Node {
        explicit Node(const T& value) : val_(value) {
        }
        T val_{};
        Node* left_ = nullptr;
        Node* right_ = nullptr;
    };
    Node* root_ = nullptr;
    void DeleteSubtree(Node* node);

    template <class UnaryFunction>
    void InordTraversal(Node* node, UnaryFunction f);
};
template <class T, class Less>
Tree<T, Less>::Tree(const T& val) : root_(new Node(val)) {
}

template <class T, class Less>
void Tree<T, Less>::DeleteSubtree(Tree::Node* node) {
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

template <class T, class Less>
Tree<T, Less>::~Tree() {
    if (root_)
        DeleteSubtree(root_);
}
template <class T, class Less>
void Tree<T, Less>::Insert(const T& value) {
    if (root_ == nullptr) {
        root_ = new Node(value);
    } else {
        Node* p = root_;
        for (Node* q = p; q != nullptr; p = q ? q : p) {
            if (Less()(value, q->val_))
                q = q->left_;
            else
                q = q->right_;
        }

        if (Less()(value, p->val_))
            p->left_ = new Node(value);
        else
            p->right_ = new Node(value);
    }
}
template <class T, class Less>
template <class UnaryFunction>
void Tree<T, Less>::InordTraversal(UnaryFunction f) {
    InordTraversal(root_, f);
}
template <class T, class Less>
template <class UnaryFunction>
void Tree<T, Less>::InordTraversal(Tree::Node* node, UnaryFunction f) {
    std::stack<Node*> s;
    Node* cur_node = node;

    while (cur_node != nullptr || !s.empty()) {
        while (cur_node != nullptr) {
            s.push(cur_node);
            cur_node = s.top()->left_;
        }
        cur_node = s.top();
        s.pop();

        f(cur_node->val_);

        cur_node = cur_node->right_;
    }
}
