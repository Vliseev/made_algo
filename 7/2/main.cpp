#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <fstream>

class UnFind {
    std::vector<int> ids_;
    std::vector<int> rank_;

    int Find(int x);

 public:
    explicit UnFind(int n);
    bool Find(int x, int y);
    void Union(int x, int y);
};

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

struct Graph {
 public:
    struct Edge {
        int u_;
        int v_;
        int w_;
        Edge(int u, int v, int w) : u_(u), v_(v), w_(w) {}
        Edge() : u_(0), v_(0), w_(0){};
    };
    std::vector<Edge> edges_{};
    int n_vertex_;

    explicit Graph(int n) : n_vertex_(n){};
    Graph(const Graph& other) = default;
    Graph(Graph&& other) noexcept = default;
    Graph& operator=(const Graph& other) = default;
    Graph& operator=(Graph&& other) noexcept = default;

    void AddEdge(int u, int v, int w);
    void SortEdges();
};

void Graph::AddEdge(int u, int v, int w) {
    edges_.emplace_back(u, v, w);
}
void Graph::SortEdges() {
    std::sort(std::begin(edges_), std::end(edges_),
              [](const Edge& e1, const Edge& e2) { return e1.w_ < e2.w_; });
}

struct MstHelper {
    const Graph& g_;
    int min_w_;

    explicit MstHelper(const Graph& graph) : g_(graph), min_w_(0){};
    void FindMst();
};
void MstHelper::FindMst() {
    UnFind uf(g_.n_vertex_);

    for (int i = 0, v = 1; i < g_.edges_.size() && v < g_.n_vertex_; ++i) {
        if (!uf.Find(g_.edges_[i].u_, g_.edges_[i].v_)) {
            uf.Union(g_.edges_[i].u_, g_.edges_[i].v_);
            min_w_ += g_.edges_[i].w_;
            v++;
        }
    }
}

int WeightMst(Graph& g) {
    g.SortEdges();
    MstHelper mst(g);
    mst.FindMst();

    return mst.min_w_;
}

Graph ReadGraph(std::istream& iss) {
    int n, m;
    iss >> n >> m;
    Graph g(n);

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        iss >> u >> v >> w;
        g.AddEdge(u - 1, v - 1, w);
    }

    return g;
}

int main() {
    std::ifstream iss("kruskal.in");
    std::ofstream oss("kruskal.out");

    Graph g = ReadGraph(iss);
    oss << WeightMst(g) << "\n";

    return 0;
}