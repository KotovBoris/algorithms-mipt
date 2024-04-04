// Найти компоненты сильной связности графа и
// топологически отсортировать его конденсацию.

#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
 public:
  Graph(const std::vector<std::vector<size_t>>& adjacency_list)
      : adjacency_list_(adjacency_list), visited_(adjacency_list.size()) {}

  Graph Reversed() {
    std::vector<std::vector<size_t>> reversed_list(adjacency_list_.size());
    for (size_t i = 0; i < adjacency_list_.size(); ++i) {
      for (size_t j = 0; j < adjacency_list_[i].size(); ++j) {
        reversed_list[adjacency_list_[i][j]].push_back(i);
      }
    }

    return Graph(reversed_list);
  }

  std::vector<size_t> FindSCCs() {
    return Reversed().ConnectivityComponents(PseudoTopSort());
  }

 private:
  std::vector<std::vector<size_t>> adjacency_list_;
  std::vector<bool> visited_;

  void SortComponent(size_t current, std::vector<size_t>& sorted) {
    if (visited_[current]) {
      return;
    }

    visited_[current] = true;

    for (size_t i = 0; i < adjacency_list_[current].size(); ++i) {
      SortComponent(adjacency_list_[current][i], sorted);
    }

    sorted.push_back(current);
  }

  std::vector<size_t> PseudoTopSort() {
    std::vector<size_t> sorted;

    for (size_t i = 0; i < adjacency_list_.size(); ++i) {
      SortComponent(i, sorted);
    }

    std::reverse(sorted.begin(), sorted.end());

    std::fill(visited_.begin(), visited_.end(), false);

    return sorted;
  }

  std::vector<size_t> ConnectivityComponents(const std::vector<size_t>& order) {
    std::vector<size_t> distribution(adjacency_list_.size());

    size_t component_number = 0;
    for (size_t i = 0; i < distribution.size(); ++i) {
      std::vector<size_t> current_component;
      SortComponent(order[i], current_component);

      if (!current_component.empty()) {
        ++component_number;

        for (size_t element : current_component) {
          distribution[element] = component_number;
        }
      }
    }

    return distribution;
  }
};

int main() {
  size_t vertices;
  size_t edges;
  std::cin >> vertices >> edges;

  std::vector<std::vector<size_t>> adjacency_list(vertices);

  for (size_t i = 0; i < edges; ++i) {
    size_t start;
    size_t end;
    std::cin >> start >> end;

    adjacency_list[start - 1].push_back(end - 1);
  }

  Graph graph(adjacency_list);

  std::vector<size_t> components = graph.FindSCCs();
  std::cout << *std::max_element(components.begin(), components.end()) << '\n';

  for (auto element : components) {
    std::cout << element << ' ';
  }
}
