//  Построить в двудольном графе минимальное вершинное покрытие, если дано
//  максимальное паросочетание.

#include <iostream>
#include <vector>

class BipartiteGraph {
 public:
  explicit BipartiteGraph(size_t left_size, size_t right_size)
      : left_size_(left_size), right_size_(right_size), adj_list_(left_size) {}

  void AddEdge(size_t from_left, size_t to_right) {
    adj_list_[from_left].push_back(to_right);
  }

  size_t LeftSize() const { return left_size_; }

  size_t RightSize() const { return right_size_; }

  const std::vector<size_t>& GetNeighbours(size_t vertex) const {
    return adj_list_[vertex];
  }

 private:
  size_t left_size_;
  size_t right_size_;
  std::vector<std::vector<size_t>> adj_list_;
};

struct Coverage {
  std::vector<size_t> left;
  std::vector<size_t> right;
};

class CoverageFinder {
 public:
  explicit CoverageFinder(const BipartiteGraph& graph,
                          const std::vector<int>& matching)
      : graph_(graph),
        left_visited_(graph.LeftSize(), false),
        right_visited_(graph.RightSize(), false),
        matching_(matching) {}

  Coverage FindMinCoverage() {
    std::vector<bool> is_left_saturated(graph_.LeftSize(), false);
    for (size_t right = 0; right < graph_.RightSize(); ++right) {
      if (matching_[right] != -1) {
        is_left_saturated[matching_[right]] = true;
      }
    }

    for (size_t left = 0; left < graph_.LeftSize(); ++left) {
      if (!is_left_saturated[left]) {
        DfsFromUnsaturated(left);
      }
    }

    return ExtractCoverage();
  }

 private:
  void DfsFromUnsaturated(size_t vertex) {
    if (left_visited_[vertex]) {
      return;
    }

    left_visited_[vertex] = true;

    for (size_t right : graph_.GetNeighbours(vertex)) {
      if (matching_[right] != static_cast<int>(vertex)) {
        right_visited_[right] = true;
        DfsFromUnsaturated(matching_[right]);
      }
    }
  }

  Coverage ExtractCoverage() const {
    Coverage coverage;

    for (size_t i = 0; i < graph_.LeftSize(); ++i) {
      if (!left_visited_[i]) {
        coverage.left.push_back(i);
      }
    }

    for (size_t i = 0; i < graph_.RightSize(); ++i) {
      if (right_visited_[i]) {
        coverage.right.push_back(i);
      }
    }

    return coverage;
  }

  const BipartiteGraph& graph_;
  std::vector<bool> left_visited_;
  std::vector<bool> right_visited_;
  std::vector<int> matching_;
};

int main() {
  size_t left_size;
  size_t right_size;
  std::cin >> left_size >> right_size;

  BipartiteGraph graph(left_size, right_size);
  for (size_t i = 0; i < left_size; ++i) {
    size_t incident_count;
    std::cin >> incident_count;
    for (size_t j = 0; j < incident_count; ++j) {
      size_t right_vertex;
      std::cin >> right_vertex;
      graph.AddEdge(i, right_vertex - 1);
    }
  }

  std::vector<int> matching(right_size, -1);
  for (size_t i = 0; i < left_size; ++i) {
    size_t left_vertex;
    std::cin >> left_vertex;
    if (left_vertex != 0) {
      matching[left_vertex - 1] = i;
    }
  }

  CoverageFinder finder(graph, matching);
  Coverage coverage = finder.FindMinCoverage();

  std::cout << coverage.left.size() + coverage.right.size() << "\n";
  std::cout << coverage.left.size() << " ";
  for (auto vertex : coverage.left) {
    std::cout << vertex + 1 << " ";
  }
  std::cout << "\n";

  std::cout << coverage.right.size() << " ";
  for (auto vertex : coverage.right) {
    std::cout << vertex + 1 << " ";
  }
  std::cout << "\n";

  return 0;
}
