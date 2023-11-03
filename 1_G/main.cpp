/*Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше
  10**6. Отсортировать массив методом поразрядной сортировки LSD по байтам.*/

#include <cmath>
#include <iostream>
#include <vector>

size_t GetByte(size_t number, size_t bit_num) {
  const size_t kBitsNumInByte = 8;
  const size_t kMaxByte = 8;
  number <<= kBitsNumInByte * (kMaxByte - bit_num);
  number >>= kMaxByte * kBitsNumInByte - kBitsNumInByte;
  return number;
}

void RadixSort(std::vector<size_t>& array) {
  const size_t kBitsNumInByte = 8;
  size_t lenght = array.size();
  for (size_t bit_num = 1; bit_num <= kBitsNumInByte; ++bit_num) {
    std::vector<size_t> count(pow(2, kBitsNumInByte));
    for (size_t i = 0; i < lenght; ++i) {
      ++count[GetByte(array[i], bit_num)];
    }
    for (size_t i = 1; i < pow(2, kBitsNumInByte); ++i) {
      count[i] += count[i - 1];
    }
    std::vector<size_t> sorted_array(lenght);
    for (size_t i = lenght; i > 0; --i) {
      sorted_array[--count[GetByte(array[i - 1], bit_num)]] = array[i - 1];
    }

    array = sorted_array;
  }
}

int main() {
  size_t lenght;
  std::cin >> lenght;
  std::vector<size_t> array(lenght);
  for (size_t i = 0; i < lenght; ++i) {
    std::cin >> array[i];
  }

  RadixSort(array);

  for (size_t element : array) {
    std::cout << element << '\n';
  }
}
