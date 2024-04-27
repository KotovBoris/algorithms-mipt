//  Найти отрицательный цикл в графе

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

void GetPath(std::vector<size_t>& path, size_t first, size_t second,
             const std::vector<std::vector<int>>& node_between) {
  if (node_between[first][second] == -1) {
    path.push_back(first);
    return;
  }

  size_t between = node_between[first][second];
  GetPath(path, first, between, node_between);
  GetPath(path, between, second, node_between);
}

std::vector<size_t> NegativeCycle(std::vector<std::vector<int>> distances) {
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
          std::vector<size_t> cycle;
          GetPath(cycle, start, end, node_between);
          GetPath(cycle, end, start, node_between);
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

  int loop = -1;

  for (size_t i = 0; i < number; ++i) {
    for (size_t j = 0; j < number; ++j) {
      std::cin >> adjacency_matrix[i][j];

      constexpr int cInfinity = std::numeric_limits<int>::max() / 2;
      const int cBadInfinity = 100000;
      if (adjacency_matrix[i][j] == cBadInfinity) {
        adjacency_matrix[i][j] = cInfinity;
      }

      if (i == j && adjacency_matrix[i][j] < 0) {
        loop = i;
      }
    }
  }

  if (loop != -1) {
    std::cout << "YES\n2\n" << loop + 1 << ' ' << loop + 1;
    return 0;
  }

  std::vector<size_t> cycle = NegativeCycle(adjacency_matrix);

  if (cycle.empty()) {
    std::cout << "NO";
    return 0;
  }

  std::cout << "YES\n" << cycle.size() << '\n';
  for (auto node : cycle) {
    std::cout << node + 1 << ' ';
  }
}
