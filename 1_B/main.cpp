/*В первой строке дана длина массива ∊ [1; 3*10**5]
На второй строке - массив из вещественных чисел (0.01 ≤ ai ≤ 10**3) с двумя знаками после десятичной точки.
На третьей строке - кол-во запросов ∊ [1; 10**5].
Далее идут запросы в формате «i j» (0 ≤ i ≤ j ≤ N - 1): i, j — индексы массива, задающие отрезок для запроса.

Вывести для каждого запроса среднее геометрическое на подмассиве [i, j].*/

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

void CreatePrefix(std::vector<double>& volumes,
                  std::vector<double>& prefix_geometric_avg) {
  size_t lenght = prefix_geometric_avg.size();
  double current_geometric_avg = 1;
  for (size_t i = 0; i < lenght; ++i) {
    current_geometric_avg *= pow(volumes[i], 1.0 / lenght);
    prefix_geometric_avg[i] = current_geometric_avg;
  }
}

void CreateSuffix(std::vector<double>& volumes,
                  std::vector<double>& suffix_geometric_avg) {
  size_t lenght = suffix_geometric_avg.size();
  double current_geometric_avg = 1;
  for (size_t i = lenght; i > 0; --i) {
    current_geometric_avg *= pow(volumes[i - 1], 1.0 / lenght);
    suffix_geometric_avg[i - 1] = current_geometric_avg;
  }
}

double GetGeometricAvgOnSubarray(std::vector<double>& prefix_geometric_avg,
                                 std::vector<double>& suffix_geometric_avg,
                                 size_t left, size_t right) {
  size_t len = prefix_geometric_avg.size();
  double ans = prefix_geometric_avg[len - 1];
  if (left != 0) {
    ans /= prefix_geometric_avg[left - 1];
  }
  if (right + 1 != len) {
    ans /= suffix_geometric_avg[right + 1];
  }
  return pow(ans, len / (right - left + 1.0));
}

int main() {
  size_t lenght;
  std::cin >> lenght;
  std::vector<double> volumes(lenght);
  for (size_t i = 0; i < lenght; ++i) {
    std::cin >> volumes[i];
  }

  std::vector<double> prefix_geometric_avg(lenght);
  std::vector<double> suffix_geometric_avg(lenght);
  CreatePrefix(volumes, prefix_geometric_avg);
  CreateSuffix(volumes, suffix_geometric_avg);

  size_t requests_num;
  std::cin >> requests_num;
  size_t left;
  size_t right;
  for (size_t i = 0; i < requests_num; ++i) {
    std::cin >> left >> right;
    const size_t kSetprecision = 10;
    std::cout << std::fixed << std::setprecision(kSetprecision)
              << GetGeometricAvgOnSubarray(prefix_geometric_avg,
                                           suffix_geometric_avg, left, right)
              << '\n';
  }
}
