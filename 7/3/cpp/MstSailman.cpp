//
// Created by vlad on 28.12.2019.
//
#include "MstSailman.h"
#include <algorithm>
#include <stack>
#include "UnFind.h"

void MstHelper::FindMst() {
    UnFind uf(n_vert_);

    for (int i = 0, v = 1; i < edges_.size() && v < n_vert_; ++i) {
        if (!uf.Find(edges_[i].u_, edges_[i].v_)) {
            uf.Union(edges_[i].u_, edges_[i].v_);
            min_w_ += edges_[i].w_;
            mst_.emplace_back(edges_[i]);
            v++;
        }
    }
}

double SailmanWeight(const graph::Graph& g, const graph::Edges& edges) {
    double tot_w = 0;
    std::vector<bool> visited(g.n_vertex_, false);
    std::stack<std::pair<int, double>> stack;

    auto first_v = std::find_if(g.edges_.begin(), g.edges_.end(),
                                [](auto e) { return !e.empty(); });
    auto v_id = std::distance(g.edges_.begin(), first_v);
    stack.push({v_id, 0.0});

    auto last_v = 0;
    while (!stack.empty()) {
        auto [v, w] = stack.top();
        stack.pop();
        last_v = v;
        visited[v] = true;
        tot_w += w;

        for (auto [u, w] : g.edges_.at(v)) {
            if (!visited[u])
                stack.push({u, w});
        }
    }

    for (auto& v : edges) {
        if (v.u_ == v_id && v.v_ == last_v) {
            tot_w += v.w_;
            break;
        }
    }
    return tot_w;
}

double SailmanWeightMst(graph::Edges& edges, int n_vert) {
    std::sort(edges.begin(), edges.end(),
              [](auto u, auto v) { return u.w_ < v.w_; });

    MstHelper mst(edges, n_vert);
    mst.FindMst();

    graph::Graph tree(n_vert);

    for (auto& l : mst.mst_) {
        tree.AddEdge(l.u_, l.v_, l.w_);
    }

    return SailmanWeight(tree, edges);
}