//  Рюкзак:
//  Выбрать из множества поручений такой набор, чтобы суммарные ресурсы
//  не превосходили заданной величины, а суммарная награда была максимальна. 

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

std::vector<size_t> OptimalSet(size_t max_resourses,
                               const std::vector<size_t>& resourses,
                               const std::vector<size_t>& awards) {
  size_t length = awards.size();
  std::vector<std::vector<size_t>> dp(length + 1,
                                      std::vector<size_t>(max_resourses + 1));
  std::vector<std::vector<bool>> taken(length + 1,
                                       std::vector<bool>(max_resourses + 1));

  for (size_t i = 0; i < length; ++i) {
    for (size_t j = 0; j <= max_resourses; ++j) {
      dp[i + 1][j] = std::max(dp[i + 1][j], dp[i][j]);

      if (j + resourses[i] <= max_resourses) {
        dp[i + 1][j + resourses[i]] =
            std::max(dp[i + 1][j + resourses[i]], dp[i][j] + awards[i]);
      }
    }
  }

  size_t current = std::max_element(dp[length].begin(), dp[length].end()) -
                   dp[length].begin();

  std::vector<size_t> set;
  for (size_t i = length; i >= 1; --i) {
    if (dp[i][current] != dp[i - 1][current]) {
      set.push_back(i);
      current -= resourses[i - 1];
    }
  }

  std::reverse(set.begin(), set.end());
  return set;
}

int main() {
  size_t number;
  size_t max_resourses;
  std::cin >> number >> max_resourses;

  std::vector<size_t> resourses(number);
  for (size_t i = 0; i < number; ++i) {
    std::cin >> resourses[i];
  }

  std::vector<size_t> awards(number);
  for (size_t i = 0; i < number; ++i) {
    std::cin >> awards[i];
  }

  std::vector<size_t> set = OptimalSet(max_resourses, resourses, awards);

  for (size_t element : set) {
    std::cout << element << '\n';
  }
}
