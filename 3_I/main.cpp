#include <algorithm>
#include <iostream>
#include <vector>

std::pair<int, int> Merge(std::pair<int, int> first_pair,
                          std::pair<int, int> second_pair,
                          std::vector<int>& sequence) {
  std::vector<std::pair<int, int>> statistics = {
      {sequence[first_pair.first], first_pair.first},
      {sequence[first_pair.second], first_pair.second},
      {sequence[second_pair.first], second_pair.first},
      {sequence[second_pair.second], second_pair.second}};
  std::sort(statistics.begin(), statistics.end());
  statistics.erase(unique(statistics.begin(), statistics.end()),
                   statistics.end());
  return {statistics[0].second, statistics[1].second};
}

int SecondStatistic(size_t left, size_t right,
                    std::vector<std::vector<std::pair<int, int>>>& sparse_table,
                    std::vector<size_t>& degree2, std::vector<int>& sequence) {
  size_t degree = degree2[right - left + 1];
  return Merge(sparse_table[degree][left],
               sparse_table[degree][right - (1 << degree) + 1], sequence)
      .second;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  size_t length;
  size_t request_number;
  std::cin >> length >> request_number;

  std::vector<int> sequence(length + 1);
  for (size_t i = 0; i < length; ++i) {
    std::cin >> sequence[i];
  }

  std::vector<size_t> degree2(length + 1);
  degree2[1] = 0;
  for (size_t i = 2; i < length + 1; ++i) {
    degree2[i] = degree2[i - 1];
    if ((i & (i - 1)) == 0) {
      degree2[i]++;
    }
  }

  std::vector<std::vector<std::pair<int, int>>> sparse_table(degree2[length] +
                                                             1);

  const int kInfinity = 2000000000;
  sequence[length] = kInfinity;

  sparse_table[0].resize(length);
  for (size_t i = 0; i < length; ++i) {
    sparse_table[0][i].first = i;
    sparse_table[0][i].second = length;
  }

  for (size_t degree = 1; degree < degree2[length] + 1; ++degree) {
    sparse_table[degree].resize(length - (1 << degree) + 1);
    for (size_t i = 0; i < length - (1 << degree) + 1; ++i) {
      std::pair<int, int> merged =
          Merge(sparse_table[degree - 1][i],
                sparse_table[degree - 1][i + (1 << (degree - 1))], sequence);
      sparse_table[degree][i].first = merged.first;
      sparse_table[degree][i].second = merged.second;
    }
  }

  for (size_t i = 0; i < request_number; ++i) {
    size_t left;
    size_t right;
    std::cin >> left >> right;
    std::cout << sequence[SecondStatistic(--left, --right, sparse_table,
                                          degree2, sequence)]
              << '\n';
  }
}
