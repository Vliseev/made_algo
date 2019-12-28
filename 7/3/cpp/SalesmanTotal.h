//
// Created by vlad on 28.12.2019.
//

#pragma once
#include <valarray>
#include <vector>
#include "Graph.h"

template <typename T>
class Matrix {
    std::valarray<T> data;
    int dim;

   public:
    Matrix(int r, int c) : data(r * c), dim(c) {
    }
    T& operator()(int r, int c) {
        return data[r * dim + c];
    }
};

struct SalesManTotal {
    double ans;
    Matrix<double > w;
    std::vector<int> p;
    std::vector<bool> used;
    int n;
    SalesManTotal(const graph::Edges& edges, int n);
    void rec(int idx, double len);
};