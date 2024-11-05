#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using VecST = std::vector<size_t>;

constexpr size_t cAlphabetSize = 26;
constexpr char cFirstSymbol = 'a';  //  don't use '\0'

constexpr char cSpecialSymbol = static_cast<char>(cFirstSymbol - 1);

class AlphabetIndex {
  const std::string& str_;

 public:
  size_t range = cAlphabetSize + 1;

  AlphabetIndex(const std::string& str) : str_(str){};

  size_t operator()(size_t index) const { return str_[index] - cSpecialSymbol; }
};

class ShiftedEqClass {
  const VecST& eq_class_;
  size_t shift_;

 public:
  size_t range;

  ShiftedEqClass(const VecST& eq_class, size_t shift)
      : eq_class_(eq_class), shift_(shift) {
    range = *std::max_element(eq_class.begin(), eq_class.end()) + 1;
  }

  size_t operator()(size_t index) const {
    return eq_class_[(index + shift_) % eq_class_.size()];
  }
};

class PairEqClass {
  ShiftedEqClass non_shifted_;
  ShiftedEqClass shifted_;

 public:
  size_t range;

  PairEqClass(const VecST& eq_class, size_t shift)
      : non_shifted_(eq_class, 0), shifted_(eq_class, shift) {
    range = non_shifted_.range * shifted_.range + shifted_.range;
  }

  size_t operator()(size_t index) const {
    return non_shifted_(index) * shifted_.range + shifted_(index);
  }
};

template <typename T, typename Value>
VecST FindValueArr(std::vector<T> array, const Value& value) {
  VecST value_arr(array.size());

  for (size_t i = 0; i < array.size(); ++i) {
    value_arr[i] = value(array[i]);
  }

  return value_arr;
}

template <typename T, typename Value>
void CountingSort(std::vector<T>& array, const Value& value) {  //  stable
  VecST count(value.range);
  VecST value_arr = FindValueArr(array, value);

  for (size_t i = 0; i < array.size(); ++i) {
    ++count[value_arr[i]];
  }

  for (size_t i = 1; i < count.size(); ++i) {
    count[i] += count[i - 1];
  }

  std::vector<T> copy = array;

  for (int i = static_cast<int>(array.size() - 1); i >= 0; --i) {
    array[--count[value_arr[i]]] = copy[i];
  }
}

template <typename Value>
VecST FindEqClass(const VecST& sorted_suffixes, const Value& value) {
  VecST value_arr = FindValueArr(sorted_suffixes, value);

  VecST eq_class(sorted_suffixes.size());
  eq_class[0] = 0;

  for (size_t i = 1; i < eq_class.size(); ++i) {
    eq_class[sorted_suffixes[i]] = (value_arr[i - 1] == value_arr[i])
                                       ? eq_class[sorted_suffixes[i - 1]]
                                       : eq_class[sorted_suffixes[i - 1]] + 1;
  }

  return eq_class;
}

VecST CyclicArray(std::string& str) {
  VecST sorted_cycles(str.size());
  for (size_t i = 0; i < sorted_cycles.size(); ++i) {
    sorted_cycles[i] = i;
  }

  CountingSort(sorted_cycles, AlphabetIndex(str));

  VecST eq_class(str.size());
  eq_class = FindEqClass(sorted_cycles, AlphabetIndex(str));

  size_t current_len = 1;
  while (current_len < str.size()) {
    CountingSort(sorted_cycles, ShiftedEqClass(eq_class, current_len));
    CountingSort(sorted_cycles, ShiftedEqClass(eq_class, 0));

    eq_class = FindEqClass(sorted_cycles, PairEqClass(eq_class, current_len));

    current_len *= 2;
  }

  return sorted_cycles;
}

int main() {
  std::string str;
  std::cin >> str;

  VecST cyclic_array = CyclicArray(str);

  for (auto element : cyclic_array) {
    std::cout << str[(element + (str.size() - 1)) % str.size()];
  }

  return 0;
}
