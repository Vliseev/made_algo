//
// Created by vlad on 28.12.2019.
//

#pragma once
#include <vector>

class UnFind {
    std::vector<int> ids_;
    std::vector<int> rank_;

    int Find(int x);

   public:
    explicit UnFind(int n);
    bool Find(int x, int y);
    void Union(int x, int y);
};

