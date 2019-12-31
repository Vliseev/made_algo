#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>

struct PHasher {
    size_t operator()(std::pair<int, int> p) const {
        return std::hash<int>()(p.first) << 3 ^ std::hash<int>()(p.first) >> 7;
    }
};

struct Graph {
    std::vector<std::vector<int>> edges;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, PHasher>
        egde_counter;
};

void ReadGraph(Graph& graph, std::istream& iss) {
    graph.edges.clear();
    int n, m;
    iss >> n >> m;

    graph.edges.resize(n);

    for (int i = 0; i < m; ++i) {
        int w, v;
        iss >> w >> v;
        w--, v--;
        if (w != v) {
            graph.edges[w].push_back(v);
            graph.edges[v].push_back(w);
            graph.egde_counter[{w, v}].second++;
            graph.egde_counter[{v, w}].second++;

            graph.egde_counter[{w, v}].first = i;
            graph.egde_counter[{v, w}].first = i;
        } else {
            graph.edges[w].push_back(v);
            graph.egde_counter[{v, w}].second++;

            graph.egde_counter[{v, w}].first = i;
        }
    }
    for (int i = 0; i < n; ++i) {
        std::sort(graph.edges[i].begin(), graph.edges[i].end());
        auto last = std::unique(graph.edges[i].begin(), graph.edges[i].end());
        graph.edges[i].erase(last, graph.edges[i].end());
    }
}

enum class Color { white, gray, black };

struct DfsHelper {
    struct StackFrame {
        int vertex;
        int child_idx;
        int parent;
    };

    const Graph& g_;
    std::vector<Color> color_;
    std::vector<int> entry_;
    std::vector<int> lowest_;

    std::vector<int> bridges_{};

    void UpdateBridge(int i, int j);
    int time = 0;

    explicit DfsHelper(const Graph& g) : g_(g) {
        auto n_v = g.edges.size();
        color_.assign(n_v, Color::white);
        entry_.resize(n_v);
        lowest_.resize(n_v);
    }

    void DfsIter(int vertex);
};

void DfsHelper::DfsIter(int vertex) {
    std::stack<StackFrame> st;
    auto& graph = g_.edges;

    st.push({vertex, -1, -1});
    color_[vertex] = Color::gray;
    entry_[vertex] = lowest_[vertex] = time++;

    while (!st.empty()) {
        auto& [v, idx, parent] = st.top();

        idx += 1;
        if (idx < graph[v].size()) {
            auto w = graph[v][idx];

            if (color_[w] == Color::white) {
                entry_[w] = lowest_[w] = time++;

                color_[w] = Color::gray;
                st.push({w, -1, v});
            } else if (w != parent) {
                lowest_[v] = std::min(lowest_[v], entry_[w]);
            }
        } else {
            color_[v] = Color::black;
            st.pop();
            if (!st.empty()) {
                auto top_val = st.top();
                auto u = graph[top_val.vertex][top_val.child_idx];
                auto cur_v = top_val.vertex;

                lowest_[cur_v] = std::min(lowest_[cur_v], lowest_[u]);
                if (lowest_[u] > entry_[cur_v]) {
                    UpdateBridge(cur_v, u);
                }
            }
        }
    }
}

void DfsHelper::UpdateBridge(int i, int j) {
    if (g_.egde_counter.at({i, j}).second > 1)
        return;
    bridges_.push_back(g_.egde_counter.at({i, j}).first);
}

void FindBridgeIt(Graph& g) {
    DfsHelper dfs(g);
    int num_vertex = g.edges.size();

    for (int i = 0; i < num_vertex; ++i) {
        if (dfs.color_[i] == Color::white)
            dfs.DfsIter(i);
    }

    std::ofstream oss("bridges.out");
    oss << dfs.bridges_.size() << "\n";
    std::sort(dfs.bridges_.begin(), dfs.bridges_.end());

    for (auto el : dfs.bridges_)
        oss << el + 1 << "\n";
}

int main() {
    std::ifstream iss("bridges.in");

    Graph g;
    ReadGraph(g, iss);
    FindBridgeIt(g);

    return 0;
}
