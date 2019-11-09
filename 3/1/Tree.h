//
// Created by user on 05.11.2019.
//

#pragma once
#include <iostream>
#include <stack>

template <class T>
class Tree {
 public:
    struct TreeNode {
        explicit TreeNode(const T& value) : val_(value) {}
        T val_{};
        TreeNode* left_ = nullptr;
        TreeNode* right_ = nullptr;
    };

    ~Tree();
    explicit Tree(const T& val);
    void Insert(const T& value);
    Tree() = default;
    void PrintInorder(std::ostream& oss);

 private:
    TreeNode* root_ = nullptr;
    void DeleteSubtree(TreeNode* node);
    void PrintSubTree(TreeNode* node, std::ostream& oss);
};
template <class T>
Tree<T>::Tree(const T& val) : root_(new TreeNode(val)) {}

template <class T>
void Tree<T>::DeleteSubtree(Tree::TreeNode* node) {
    std::stack<TreeNode*> s;
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

template <class T>
Tree<T>::~Tree() {
    if (root_)
        DeleteSubtree(root_);
}
template <class T>
void Tree<T>::Insert(const T& value) {
    if (root_ == nullptr) {
        root_ = new TreeNode(value);
    } else {
        TreeNode* p = root_;
        for (TreeNode* q = p; q != nullptr; p = q ? q : p) {
            if (value <= q->val_)
                q = q->left_;
            else if (value > q->val_)
                q = q->right_;
        }

        if (value < p->val_)
            p->left_ = new TreeNode(value);
        else
            p->right_ = new TreeNode(value);
    }
}
template <class T>
void Tree<T>::PrintSubTree(Tree::TreeNode* node, std::ostream& oss) {
    std::stack<TreeNode*> s;
    TreeNode* cur_node = node;

    while (cur_node != nullptr || !s.empty()) {
        while (cur_node != nullptr) {
            s.push(cur_node);
            cur_node = s.top()->left_;
        }
        cur_node = s.top();
        s.pop();

        oss << cur_node->val_ << " ";

        cur_node = cur_node->right_;
    }
}
template <class T>
void Tree<T>::PrintInorder(std::ostream& oss) {
    PrintSubTree(root_, oss);
}
