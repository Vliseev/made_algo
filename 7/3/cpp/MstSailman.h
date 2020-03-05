//
// Created by vlad on 28.12.2019.
//
#pragma once
#include "Graph.h"

struct MstHelper {
    const graph::Edges& edges_;
    double min_w_;
    size_t n_vert_;
    graph::Edges mst_{};

    explicit MstHelper(const graph::Edges& edges, size_t n_vert)
        : edges_(edges), min_w_(0), n_vert_(n_vert){};
    void FindMst();
};

double SailmanWeight(const graph::Graph& g, const graph::Edges& edges);
double SailmanWeightMst(graph::Edges& edges, int n_vert);