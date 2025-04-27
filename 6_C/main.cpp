//  Найти минимальное количество экспериментов, которое потребуется совершить,
//  чтобы узнать максимальную высоту, падение с которой самолётик переживет

#include <iostream>
#include <limits>
#include <vector>

long long AfterDrop(size_t number, size_t height, size_t drop_from,
                    const std::vector<std::vector<long long>>& dp) {
  return std::max(dp[number - 1][drop_from], dp[number][height - drop_from]);
}

long long MinDropsCount(size_t max_height, size_t helicopters) {
  size_t halfing_count = 0;
  size_t copy = max_height;
  while (copy > 1) {
    copy = copy / 2 + copy % 2;
    ++halfing_count;
  }

  if (halfing_count <= helicopters) {
    return halfing_count;
  }

  std::vector<std::vector<long long>> dp(
      helicopters + 1, std::vector<long long>(max_height + 1));
  //  dp[i][j] - ответ на задачу при i самолетиках и макс высоте j
  //  база - при высоте = 1 ответ 0 (уже инициализировали нулями)
  //         при 0 самолетиках невозможно => -1 (кроме высоты = 1),
  //         при 1 скидываем с 1,2,3 и тд пока не сломается => высота - 1:
  for (size_t i = 1; i <= max_height; ++i) {
    dp[0][i] = -1;
    if (helicopters >= 1) {
      dp[1][i] = i - 1;
    }
  }
  dp[0][1] = 0;

  for (size_t number = 2; number <= helicopters; ++number) {
    size_t optimal = 1;
    for (size_t height = 2; height <= max_height; ++height) {
      while (optimal <= height - 1 &&
             AfterDrop(number, height, optimal + 1, dp) <
                 AfterDrop(number, height, optimal, dp)) {
        ++optimal;
      }

      dp[number][height] = AfterDrop(number, height, optimal, dp) + 1;
    }
  }

  return dp[helicopters][max_height];
}

int main() {
  size_t max_height;
  size_t helicopters;
  std::cin >> max_height >> helicopters;

  std::cout << MinDropsCount(max_height, helicopters);
}
