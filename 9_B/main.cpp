//  Найти в связном графе остовное дерево минимального веса.

#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  size_t start_vertex;
  size_t end_vertex;
  int weight;

  bool operator<(const Edge& other) const { return weight < other.weight; }
};

class Graph {
 public:
  Graph() = default;

  void AddEdge(size_t start_vertex, size_t end_vertex, int weight) {
    edges_.push_back({start_vertex, end_vertex, weight});
  }

  const std::vector<Edge>& GetEdges() const { return edges_; }

  size_t Size() const { return edges_.size(); }

  int TotalWeight() const {
    int weight = 0;
    for (const auto& edge : edges_) {
      weight += edge.weight;
    }
    return weight;
  }

 private:
  std::vector<Edge> edges_;
};

class DisjointSetUnion {
 public:
  explicit DisjointSetUnion(size_t size) {
    parent_.resize(size);
    sizes_.resize(size, 1);
    for (size_t i = 0; i < size; ++i) {
      parent_[i] = i;
    }
  }

  int Find(size_t node) {
    if (parent_[node] != node) {
      parent_[node] = Find(parent_[node]);
    }
    return parent_[node];
  }

  void Union(size_t first, size_t second) {
    int first_root = Find(first);
    int second_root = Find(second);

    if (first_root == second_root) {
      return;
    }

    if (sizes_[first_root] < sizes_[second_root]) {
      parent_[first_root] = second_root;
      sizes_[second_root] += sizes_[first_root];
    } else {
      parent_[second_root] = first_root;
      sizes_[first_root] += sizes_[second_root];
    }
  }

 private:
  std::vector<size_t> parent_;
  std::vector<size_t> sizes_;
};

Graph FindMST(const Graph& graph, size_t num_vertices) {
  std::vector<Edge> edges_sorted = graph.GetEdges();
  std::sort(edges_sorted.begin(), edges_sorted.end());

  DisjointSetUnion dsu(num_vertices);
  Graph mst;

  for (const Edge& edge : edges_sorted) {
    if (dsu.Find(edge.start_vertex) != dsu.Find(edge.end_vertex)) {
      dsu.Union(edge.start_vertex, edge.end_vertex);
      mst.AddEdge(edge.start_vertex, edge.end_vertex, edge.weight);
    }
  }

  return mst;
}

int main() {
  size_t num_vertices;
  size_t num_edges;
  std::cin >> num_vertices >> num_edges;

  Graph graph;
  for (size_t i = 0; i < num_edges; ++i) {
    size_t start_vertex;
    size_t end_vertex;
    int weight;
    std::cin >> start_vertex >> end_vertex >> weight;
    graph.AddEdge(start_vertex - 1, end_vertex - 1, weight);
  }

  Graph mst = FindMST(graph, num_vertices);
  std::cout << mst.TotalWeight() << '\n';

  return 0;
}
