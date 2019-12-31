#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <fstream>

class DSU {
    std::vector<int> ids_;
    std::vector<int> rank_;

    int Find(int x);

   public:
    explicit DSU(int n);
    bool Find(int x, int y);
    void Union(int x, int y);
};

DSU::DSU(int n) {
    ids_.resize(n);
    rank_.resize(n);
    for (int i = 0; i < n; ++i) {
        ids_[i] = i;
        rank_[i] = 1;
    }
}

int DSU::Find(int x) {
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

bool DSU::Find(int x, int y) {
    return Find(x) == Find(y);
}

void DSU::Union(int x, int y) {
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
        Edge(int u, int v, int w) : u_(u), v_(v), w_(w) {
        }
        Edge() : u_(0), v_(0), w_(0){};
    };

   private:
    std::vector<Edge> edges_{};
    int n_vertex_;

   public:
    explicit Graph(int n) : n_vertex_(n){};
    Graph(const Graph& other) = default;
    Graph(Graph&& other) noexcept = default;
    Graph& operator=(const Graph& other) = default;
    Graph& operator=(Graph&& other) noexcept = default;

    int GetNVertex() const {
        return n_vertex_;
    }
    std::vector<Edge> CopyEdges() const {
        return edges_;
    }
    void AddEdge(int u, int v, int w);
};

void Graph::AddEdge(int u, int v, int w) {
    edges_.emplace_back(u, v, w);
}

struct MstHelper {
    const Graph& g_;
    int min_w_;

    explicit MstHelper(const Graph& graph) : g_(graph), min_w_(0){};
    void FindMst();
};
void MstHelper::FindMst() {
    DSU uf(g_.GetNVertex());

    auto edges = g_.CopyEdges();

    std::sort(std::begin(edges), std::end(edges),
              [](const Graph::Edge& e1, const Graph::Edge& e2) {
                  return e1.w_ < e2.w_;
              });

    for (int i = 0, v = 1; i < edges.size() && v < g_.GetNVertex(); ++i) {
        if (!uf.Find(edges[i].u_, edges[i].v_)) {
            uf.Union(edges[i].u_, edges[i].v_);
            min_w_ += edges[i].w_;
            v++;
        }
    }
}

int WeightMst(Graph& g) {
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