/*Вывести 2-ую порядковую статистику на диапазоне*/

#include <algorithm>
#include <iostream>
#include <vector>

class SparseTable {
 public:
  SparseTable(const std::vector<int>& data);

  int SecondStatistic(size_t left, size_t right);

 private:
  std::vector<int> data_;
  std::vector<size_t> degree2_;
  std::vector<std::vector<std::pair<int, int>>> segment_statistics_;

  void CalculateDegree2() {
    degree2_[1] = 0;
    for (size_t i = 2; i < degree2_.size(); ++i) {
      degree2_[i] = degree2_[i - 1];

      if ((i & (i - 1)) == 0) {
        degree2_[i]++;
      }
    }
  }

  std::pair<int, int> MergeStatistics(size_t degree, size_t first_index,
                                      size_t second_index) {
    std::pair<int, int> first = segment_statistics_[degree][first_index];
    std::pair<int, int> second = segment_statistics_[degree][second_index];

    std::vector<std::pair<int, int>> statistics = {
        {data_[first.first], first.first},
        {data_[first.second], first.second},
        {data_[second.first], second.first},
        {data_[second.second], second.second}};

    std::sort(statistics.begin(), statistics.end());
    std::unique(statistics.begin(), statistics.end());

    return std::make_pair(statistics[0].second, statistics[1].second);
  }

  void CalculateStatistics() {
    const int kInfinity = 2000000000;
    data_.push_back(kInfinity);

    size_t length = data_.size() - 1;
    segment_statistics_[0].resize(length);
    for (size_t i = 0; i < length; ++i) {
      segment_statistics_[0][i] = std::make_pair(i, length);
    }

    for (size_t degree = 1; degree <= degree2_[length]; ++degree) {
      size_t segment_length = (1 << degree);
      size_t current_size = length - segment_length + 1;
      segment_statistics_[degree].resize(current_size);

      for (size_t i = 0; i < current_size; ++i) {
        segment_statistics_[degree][i] =
            MergeStatistics(degree - 1, i, i + segment_length / 2);
      }
    }
  }
};

SparseTable::SparseTable(const std::vector<int>& data)
    : data_(data), degree2_(data.size() + 1) {
  CalculateDegree2();
  segment_statistics_.resize(degree2_[data.size()] + 1);
  CalculateStatistics();
}

int SparseTable::SecondStatistic(size_t left, size_t right) {
  size_t degree = degree2_[right - left + 1];
  size_t segment_length = (1 << degree);
  size_t second_start = right - segment_length + 1;

  return data_[MergeStatistics(degree, left, second_start).second];
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

  SparseTable sparse_table(sequence);

  for (size_t i = 0; i < request_number; ++i) {
    size_t left;
    size_t right;
    std::cin >> left >> right;
    std::cout << sparse_table.SecondStatistic(--left, --right) << '\n';
  }
}
