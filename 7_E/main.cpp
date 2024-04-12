// Найти все мосты в графе

#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
 public:
  explicit Graph(size_t vertices = 0) : adjacency_list_(vertices) {}

  void AddEdge(size_t first, size_t second) {
    edges_ends_.push_back(second);
    adjacency_list_[first].push_back(edges_ends_.size() - 1);
    edges_ends_.push_back(first);
    adjacency_list_[second].push_back(edges_ends_.size() - 1);
  }

  size_t Size() const { return adjacency_list_.size(); }

  const std::vector<size_t>& GetAdjacentEdges(size_t vertex) const {
    return adjacency_list_[vertex];
  }

  size_t GetEdgeEnd(size_t edge) const { return edges_ends_[edge]; }

 private:
  std::vector<std::vector<size_t>> adjacency_list_;
  std::vector<size_t> edges_ends_;
};

std::istream& operator>>(std::istream& input_stream, Graph& graph) {
  size_t vertices;
  size_t edges;
  input_stream >> vertices >> edges;

  graph = Graph(vertices);

  for (size_t i = 0; i < edges; ++i) {
    size_t first;
    size_t second;
    input_stream >> first >> second;
    --first;
    --second;

    graph.AddEdge(first, second);
  }

  return input_stream;
}

size_t CheckEdge(const Graph& graph, size_t edge, size_t depth,
                 std::vector<size_t>& depths, std::vector<size_t>& bridges) {
  depths[graph.GetEdgeEnd(edge)] = depth;
  size_t highest_jump = depth;

  const std::vector<size_t>& edges =
      graph.GetAdjacentEdges(graph.GetEdgeEnd(edge));

  for (size_t i = 0; i < edges.size(); ++i) {
    size_t first = std::min(edge, edges[i]);
    size_t second = std::max(edge, edges[i]);
    if (first % 2 == 0 && second - first == 1) {
      continue;
    }

    if (depths[graph.GetEdgeEnd(edges[i])] != 0) {
      highest_jump = std::min(highest_jump, depths[graph.GetEdgeEnd(edges[i])]);
    } else {
      highest_jump = std::min(
          highest_jump, CheckEdge(graph, edges[i], depth + 1, depths, bridges));
    }
  }

  if (highest_jump == depth) {
    bridges.push_back(edge / 2);
  }

  return highest_jump;
}

void TreeBridges(const Graph& graph, size_t root, std::vector<size_t>& depths,
                 std::vector<size_t>& bridges) {
  if (depths[root] != 0) {
    return;
  }

  depths[root] = 1;

  for (size_t i = 0; i < graph.GetAdjacentEdges(root).size(); ++i) {
    size_t current_edge = graph.GetAdjacentEdges(root)[i];
    if (depths[graph.GetEdgeEnd(current_edge)] != 0) {
      continue;
    }

    CheckEdge(graph, current_edge, 2, depths, bridges);
  }
}

std::vector<size_t> FindBridges(const Graph& graph) {
  std::vector<size_t> bridges;
  std::vector<size_t> depths(graph.Size(), 0);

  for (size_t i = 0; i < graph.Size(); ++i) {
    TreeBridges(graph, i, depths, bridges);
  }

  return bridges;
}

int main() {
  Graph graph;
  std::cin >> graph;

  std::vector<size_t> bridges = FindBridges(graph);
  std::sort(bridges.begin(), bridges.end());
  std::cout << bridges.size() << '\n';

  for (size_t element : bridges) {
    std::cout << element + 1 << '\n';
  }
}
