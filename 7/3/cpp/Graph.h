//
// Created by vlad on 28.12.2019.
//

#pragma once
#include <istream>
#include <vector>

namespace graph {
struct Edge {
    Edge(int u, int v, double w) : u_(u), v_(v), w_(w) {
    }
    int u_;
    int v_;
    double w_;
};

using Edges = std::vector<Edge>;

struct Graph {
   public:
    std::vector<std::vector<std::pair<int, double>>> edges_;
    int n_vertex_;

    explicit Graph(int n) : n_vertex_(n), edges_(n){};
    Graph(const Graph& other) = default;
    Graph(Graph&& other) noexcept = default;
    Graph& operator=(const Graph& other) = default;
    Graph& operator=(Graph&& other) noexcept = default;

    void AddEdge(int u, int v, double w);
};

std::pair<Edges, int> ReadEdges(std::istream& iss);

}    // namespace graph