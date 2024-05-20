//  Найти отрицательный цикл в графе

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

//  в матрице смежности это значение => между вершинами нет ребра
constexpr int cNoEdge = 100000;

//  делим пополам, чтобы избежать переполнения при сложении
constexpr int cInfinity = std::numeric_limits<int>::max() / 2;

void RestoreMinPath(std::vector<size_t>& path, size_t from, size_t to,
                    const std::vector<std::vector<int>>& node_between) {
  if (node_between[from][to] == -1 || from == to) {
    path.push_back(from);
    return;
  }

  size_t between = node_between[from][to];
  RestoreMinPath(path, from, between, node_between);
  RestoreMinPath(path, between, to, node_between);
}

std::vector<size_t> FindNegativeCycle(std::vector<std::vector<int>> distances) {
  size_t size = distances.size();
  std::vector<std::vector<int>> node_between(size, std::vector<int>(size, -1));

  for (size_t max_node = 0; max_node < size; ++max_node) {
    for (size_t start = 0; start < size; ++start) {
      for (size_t end = 0; end < size; ++end) {
        int& to_update = distances[start][end];
        int new_distance =
            distances[start][max_node] + distances[max_node][end];
        if (new_distance < to_update) {
          to_update = new_distance;

          node_between[start][end] = max_node;
        }

        if (distances[start][end] + distances[end][start] < 0) {
          if (start == end) {
            return {start, end};
          }

          std::vector<size_t> cycle;
          RestoreMinPath(cycle, start, end, node_between);
          RestoreMinPath(cycle, end, start, node_between);

          cycle.push_back(start);

          return cycle;
        }
      }
    }
  }

  return {};
}

int main() {
  size_t number;
  std::cin >> number;

  std::vector<std::vector<int>> adjacency_matrix(number,
                                                 std::vector<int>(number));

  for (size_t i = 0; i < number; ++i) {
    for (size_t j = 0; j < number; ++j) {
      std::cin >> adjacency_matrix[i][j];

      if (adjacency_matrix[i][j] == cNoEdge) {
        adjacency_matrix[i][j] = cInfinity;
      }
    }
  }

  std::vector<size_t> cycle = FindNegativeCycle(adjacency_matrix);

  if (cycle.empty()) {
    std::cout << "NO";
    return 0;
  }

  std::cout << "YES\n" << cycle.size() << '\n';
  for (auto node : cycle) {
    std::cout << node + 1 << ' ';
  }
}
