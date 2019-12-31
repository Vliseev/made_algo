#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <sstream>
#include <vector>

using GraphT = std::vector<std::vector<int>>;
constexpr int MaxInt = std::numeric_limits<int>::max();

void ReadGraph(GraphT& graph, std::istream& iss) {
    graph.clear();
    int n, m;
    iss >> n >> m;
    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int w, v;
        iss >> w >> v;
        graph[w].push_back(v);
        graph[v].push_back(w);
    }
    for (int i = 0; i < n; ++i) {
        std::sort(graph[i].begin(), graph[i].end());
        auto last = std::unique(graph[i].begin(), graph[i].end());
        graph[i].erase(last, graph[i].end());
    }
}

struct BFS_helper {
    const GraphT& g_;
    int Bfs(int vertex);
    explicit BFS_helper(const GraphT& g) : g_(g) {}
};

int BFS_helper::Bfs(int vertex) {
    std::queue<int> q;
    std::vector<int> dist(g_.size(), MaxInt);
    std::vector<int> parent(g_.size(), -1);
    int ans = MaxInt;

    q.push(vertex);
    dist[vertex] = 0;

    while (!q.empty()) {
        auto v = q.front();
        for (auto u : g_[v]) {
            if (dist[u] == MaxInt) {
                dist[u] = dist[v] + 1;
                parent[u] = v;
                q.push(u);
            } else if (parent[v] != u && parent[u] != v) {
                ans = std::min(ans, dist[u] + dist[v] + 1);
            }
        }
        q.pop();
    }
    return ans;
}

int MinLoop(GraphT& g) {
    BFS_helper dfs(g);
    int ans = MaxInt;
    for (int i = 0; i < g.size(); ++i) {
        ans = std::min(dfs.Bfs(i), ans);
    }
    return ans == MaxInt ? -1 : ans;
}

int main() {
    GraphT g;
    ReadGraph(g, std::cin);

    std::cout << MinLoop(g) << "\n";

    return 0;
}
