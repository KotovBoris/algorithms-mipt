//  Найти компоненты сильной связности графа и
//  топологически отсортировать его конденсацию.

#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
 public:
  explicit Graph(size_t size = 0) : adjacency_list_(size) {}

  explicit Graph(const std::vector<std::vector<size_t>>& adjacency_list)
      : adjacency_list_(adjacency_list) {}

  void AddEdge(size_t start_vertex, size_t end_vertex) {
    adjacency_list_[start_vertex].push_back(end_vertex);
  }

  const std::vector<size_t>& GetAdjacent(size_t node) const {
    return adjacency_list_[node];
  }

  size_t Size() const { return adjacency_list_.size(); }

  Graph Reversed() const {
    std::vector<std::vector<size_t>> reversed_list(Size());
    for (size_t i = 0; i < Size(); ++i) {
      for (auto node : adjacency_list_[i]) {
        reversed_list[node].push_back(i);
      }
    }

    return Graph(reversed_list);
  }

 private:
  std::vector<std::vector<size_t>> adjacency_list_;
};

std::istream& operator>>(std::istream& input_stream, Graph& graph) {
  size_t vertices;
  size_t edges;
  input_stream >> vertices >> edges;

  Graph temp_graph(vertices);
  for (size_t i = 0; i < edges; ++i) {
    size_t start_vertex;
    size_t end_vertex;
    input_stream >> start_vertex >> end_vertex;

    temp_graph.AddEdge(start_vertex - 1, end_vertex - 1);
  }

  graph = temp_graph;
  return input_stream;
}

void DFS(const Graph& graph, size_t node, std::vector<bool>& visited,
         std::vector<size_t>& order) {
  if (visited[node]) {
    return;
  }

  visited[node] = true;
  for (auto adjacent : graph.GetAdjacent(node)) {
    DFS(graph, adjacent, visited, order);
  }

  order.push_back(node);
}

std::vector<size_t> PseudoTopSort(const Graph& graph) {
  std::vector<size_t> order;
  std::vector<bool> visited(graph.Size(), false);

  for (size_t i = 0; i < graph.Size(); ++i) {
    if (!visited[i]) {
      DFS(graph, i, visited, order);
    }
  }

  std::reverse(order.begin(), order.end());
  return order;
}

std::vector<size_t> FindSCC(const Graph& graph) {
  std::vector<size_t> order = PseudoTopSort(graph);
  Graph reversed_graph = graph.Reversed();

  std::vector<size_t> components(graph.Size(), 0);
  std::vector<bool> visited(graph.Size(), false);
  size_t component_id = 0;

  for (auto vertex : order) {
    if (visited[vertex]) {
      continue;
    }

    std::vector<size_t> component;
    DFS(reversed_graph, vertex, visited, component);

    ++component_id;
    for (auto node : component) {
      components[node] = component_id;
    }
  }

  return components;
}

int main() {
  Graph graph;
  std::cin >> graph;

  std::vector<size_t> components = FindSCC(graph);
  std::cout << *std::max_element(components.begin(), components.end()) << '\n';

  for (auto element : components) {
    std::cout << element << ' ';
  }
}
