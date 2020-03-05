//
// Created by vlad on 28.12.2019.
//

#include "UnFind.h"
UnFind::UnFind(int n) {
    ids_.resize(n);
    rank_.resize(n);
    for (int i = 0; i < n; ++i) {
        ids_[i] = i;
        rank_[i] = 1;
    }
}

int UnFind::Find(int x) {
    std::vector<int> path{x};
    int root = ids_[x];

    while (root != path.back()) {
        path.push_back(root);
        root = ids_[root];
    }

    for (auto el : path)
        ids_[el] = root;

    return root;
}

bool UnFind::Find(int x, int y) {
    return Find(x) == Find(y);
}

void UnFind::Union(int x, int y) {
    int i = Find(x);
    int j = Find(y);
    if (i == j)
        return;
    if (rank_[i] < rank_[j]) {
        ids_[i] = j;
        rank_[j] += rank_[i];
    } else {
        ids_[j] = i;
        rank_[i] += rank_[j];
    }
}