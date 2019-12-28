//
// Created by vlad on 28.12.2019.
//

#include "Graph.h"
#include <cmath>

namespace graph {
void Graph::AddEdge(int u, int v, double w) {
    edges_[u].emplace_back(v, w);
    edges_[v].emplace_back(u, w);
}

std::pair<Edges, int> ReadEdges(std::istream& iss) {
    int n;
    iss >> n;

    Edges edges;
    std::vector<std::pair<double, double>> points;
    for (int i = 0; i < n; ++i) {
        std::pair<double, double> p;
        iss >> p.first >> p.second;
        points.emplace_back(p);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                auto dist =
                    std::sqrt((points[i].first - points[j].first) *
                                  (points[i].first - points[j].first) +
                              (points[i].second - points[j].second) *
                                  (points[i].second - points[j].second));
                edges.emplace_back(i, j, dist);
            }
        }
    }
    return std::make_pair(edges, n);
}
}    // namespace graph