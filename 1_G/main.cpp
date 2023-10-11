/*Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше
  10**6. Отсортировать массив методом поразрядной сортировки LSD по байтам.*/

#include <cmath>
#include <iostream>
#include <vector>

int GetByte(unsigned long long number, int bit_num) {
  const int kBitsNumInByte = 8;
  const int kMaxByte = 8;
  number <<= kBitsNumInByte * (kMaxByte - bit_num);
  number >>= kMaxByte * kBitsNumInByte - kBitsNumInByte;
  return number;
}

void RadixLSDSort(std::vector<unsigned long long>& array) {
  const int kBitsNumInByte = 8;
  size_t len = array.size();
  for (int bit_num = 1; bit_num <= kBitsNumInByte; ++bit_num) {
    std::vector<int> count(pow(2, kBitsNumInByte));
    for (size_t i = 0; i < len; ++i) {
      ++count[GetByte(array[i], bit_num)];
    }
    for (int i = 1; i < pow(2, kBitsNumInByte); ++i) {
      count[i] += count[i - 1];
    }
    std::vector<unsigned long long> sorted_array(len);
    for (int i = len - 1; i >= 0; --i) {
      sorted_array[--count[GetByte(array[i], bit_num)]] = array[i];
    }

    array = sorted_array;
  }
}

int main() {
  int len;
  std::cin >> len;
  std::vector<unsigned long long> array(len);
  for (int i = 0; i < len; ++i) {
    std::cin >> array[i];
  }

  RadixLSDSort(array);

  for (unsigned long long elem : array) {
    std::cout << elem << '\n';
  }
}
