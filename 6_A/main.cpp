// Найти максимальную невозрастающую подпоследовательность

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

template <typename Comparator>
class IndexComparator {
 public:
  IndexComparator(const std::vector<int>& data)
      : data_(data), comparator_(Comparator()) {}

  bool operator()(size_t first, size_t second) const {
    return comparator_(data_[first], data_[second]);
  }

 private:
  const std::vector<int>& data_;
  Comparator comparator_;
};

std::vector<int> MaxSubsequence(std::vector<int> sequence) {
  const int kMinusInfinity = std::numeric_limits<int>::min();

  size_t length = sequence.size();
  sequence.push_back(kMinusInfinity);

  std::vector<size_t> dp(length, length);

  std::vector<size_t> parent(length, 0);

  for (size_t i = 0; i < length; ++i) {
    auto to_update = std::upper_bound(
        dp.begin(), dp.end(), i, IndexComparator<std::greater<int>>(sequence));
    *to_update = i;

    if (to_update != dp.begin()) {
      parent[i] = *std::prev(to_update);
    }
  }

  auto longest = std::upper_bound(dp.rbegin(), dp.rend(), length,
                                  IndexComparator<std::less<int>>(sequence));
  size_t max_length = dp.rend() - longest;
  std::vector<int> subsequence(max_length);

  size_t current = *longest;
  for (int i = max_length - 1; i >= 0; --i) {
    subsequence[i] = current + 1;
    current = parent[current];
  }

  return subsequence;
}

int main() {
  size_t length;
  std::cin >> length;

  std::vector<int> sequence(length);
  for (size_t i = 0; i < length; ++i) {
    std::cin >> sequence[i];
  }

  std::vector<int> subsequence = MaxSubsequence(sequence);

  std::cout << subsequence.size() << '\n';
  for (auto element : subsequence) {
    std::cout << element << ' ';
  }
}
