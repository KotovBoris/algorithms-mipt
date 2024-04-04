// Найти все мосты в графе

#include <algorithm>
#include <iostream>
#include <vector>

class Graph {
 public:
  Graph(const std::vector<std::vector<size_t>>& adjacency_list,
        const std::vector<size_t>& edges_ends)
      : adjacency_list_(adjacency_list),
        edges_ends_(edges_ends),
        depths_(adjacency_list.size(), 0) {}

  std::vector<size_t> Bridges() {
    std::vector<size_t> bridges;

    for (size_t i = 0; i < adjacency_list_.size(); ++i) {
      TreeBridges(i, bridges);
    }

    std::fill(depths_.begin(), depths_.end(), 0);
    return bridges;
  }

 private:
  std::vector<std::vector<size_t>> adjacency_list_;
  std::vector<size_t> edges_ends_;
  std::vector<size_t> depths_;

  size_t CheckEdge(size_t edge, size_t depth, std::vector<size_t>& bridges) {
    depths_[edges_ends_[edge]] = depth;
    size_t highest_jump = depth;  //  ret

    std::vector<size_t>& edges = adjacency_list_[edges_ends_[edge]];

    for (size_t i = 0; i < edges.size(); ++i) {
      size_t first = std::min(edge, edges[i]);
      size_t second = std::max(edge, edges[i]);
      if (first % 2 == 0 && second - first == 1) {
        continue;
      }

      if (depths_[edges_ends_[edges[i]]] != 0) {
        highest_jump = std::min(highest_jump, depths_[edges_ends_[edges[i]]]);
      } else {
        highest_jump =
            std::min(highest_jump, CheckEdge(edges[i], depth + 1, bridges));
      }
    }

    if (highest_jump == depth) {
      bridges.push_back(edge / 2);
    }

    return highest_jump;
  }

  void TreeBridges(size_t root, std::vector<size_t>& bridges) {
    if (depths_[root] != 0) {
      return;
    }

    depths_[root] = 1;

    for (size_t i = 0; i < adjacency_list_[root].size(); ++i) {
      size_t current_edge = adjacency_list_[root][i];
      if (depths_[edges_ends_[current_edge]] != 0) {
        continue;
      }

      CheckEdge(current_edge, 2, bridges);
    }
  }
};

int main() {
  size_t vertices;
  size_t edges;
  std::cin >> vertices >> edges;

  std::vector<std::vector<size_t>> adjacency_list(vertices);
  std::vector<size_t> edges_ends;
  edges_ends.reserve(2 * edges);

  for (size_t i = 0; i < edges; ++i) {
    size_t first;
    size_t second;
    std::cin >> first >> second;
    --first;
    --second;

    edges_ends.push_back(second);
    adjacency_list[first].push_back(2 * i);
    edges_ends.push_back(first);
    adjacency_list[second].push_back(2 * i + 1);
  }

  Graph graph(adjacency_list, edges_ends);

  std::vector<size_t> bridges = graph.Bridges();
  std::sort(bridges.begin(), bridges.end());
  std::cout << bridges.size() << '\n';

  for (auto element : bridges) {
    std::cout << element + 1 << '\n';
  }
}
