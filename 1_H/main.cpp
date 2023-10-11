/*C помощью быстрой сортировки отсортировать массив.

  Написан детерминированный алгоритм (O(nlogn))*/

#include <iostream>
#include <vector>

int MedianOf1234(std::vector<int>& array) {
  if (array.size() <= 2) {
    return array[0];
  }
  if (array[0] > array[1]) {
    std::swap(array[0], array[1]);
  }
  if (array[1] > array[2]) {
    std::swap(array[1], array[2]);
  }
  return std::max(array[0], array[1]);
}

int MedianOf5(std::vector<int>& slice) {
  if (slice[0] > slice[1]) {
    std::swap(slice[0], slice[1]);
  }
  if (slice[2] > slice[3]) {
    std::swap(slice[2], slice[3]);
  }
  if (slice[0] > slice[2]) {
    std::swap(slice[0], slice[2]);
  }
  if (slice[1] < slice[3]) {
    std::swap(slice[1], slice[3]);
  }
  if (slice[2] > slice[3]) {
    std::swap(slice[2], slice[3]);
  }
  if (slice[3] > slice[4]) {
    std::swap(slice[3], slice[4]);
  }
  return std::max(slice[2], slice[3]);
}

int Partition(std::vector<int>& array, int start, int end, int pivot) {
  int index_for_less_than_pivot = start;
  int index_for_more_than_pivot = end - 1;
  while (index_for_less_than_pivot < end &&
         array[index_for_less_than_pivot] < pivot) {
    ++index_for_less_than_pivot;
  }
  while (index_for_more_than_pivot >= start &&
         array[index_for_more_than_pivot] >= pivot) {
    --index_for_more_than_pivot;
  }
  while (index_for_less_than_pivot < index_for_more_than_pivot) {
    std::swap(array[index_for_less_than_pivot],
              array[index_for_more_than_pivot]);
    while (index_for_less_than_pivot < end &&
           array[index_for_less_than_pivot] < pivot) {
      ++index_for_less_than_pivot;
    }
    while (index_for_more_than_pivot >= start &&
           array[index_for_more_than_pivot] >= pivot) {
      --index_for_more_than_pivot;
    }
  }
  return index_for_more_than_pivot + 1 - start;
}

int QuickSelect(std::vector<int>& array, int start, int end,
                int order_statistic_num, std::vector<int>& slice) {
  int len = end - start;
  if (len == 1) {
    return array[start];
  }

  const int kSliceLen = 5;
  int medians_len = len / kSliceLen;
  int last_slice_len = len % kSliceLen;
  if (last_slice_len != 0) {
    ++medians_len;
  }
  std::vector<int> medians(medians_len);
  for (int i = 0; i < len / kSliceLen; ++i) {
    for (int j = 0; j < kSliceLen; ++j) {
      slice[j] = array[start + i * kSliceLen + j];
    }
    medians[i] = MedianOf5(slice);
  }
  if (last_slice_len != 0) {
    std::vector<int> last_slice(last_slice_len);
    for (int i = 0; i < last_slice_len; ++i) {
      last_slice[i] = array[end - last_slice_len + i];
    }
    medians[medians_len - 1] = MedianOf1234(last_slice);
  }

  int pivot;
  if (medians_len < 5) {
    pivot = MedianOf1234(medians);
  } else if (medians_len == 5) {
    pivot = MedianOf5(medians);
  } else {
    pivot = QuickSelect(medians, 0, medians_len, (medians_len + 1) / 2, slice);
  }
  int less_pivot_num = Partition(array, start, end, pivot);
  if (order_statistic_num <= less_pivot_num) {
    return QuickSelect(array, start, start + less_pivot_num,
                       order_statistic_num, slice);
  }
  int less_and_equal_pivot_num =
      Partition(array, start + less_pivot_num, end, pivot + 1) + less_pivot_num;
  if (order_statistic_num <= less_and_equal_pivot_num) {
    return pivot;
  }
  return QuickSelect(array, start + less_and_equal_pivot_num, end,
                     order_statistic_num - less_and_equal_pivot_num, slice);
}

void QuickSort(std::vector<int>& array, int start, int end) {
  int len = end - start;
  if (len <= 1) {
    return;
  }
  const int kSliceLen = 5;
  std::vector<int> slice(kSliceLen);
  int pivot = QuickSelect(array, start, end, (len + 1) / 2, slice);
  int less_pivot_num = Partition(array, start, end, pivot);
  int less_and_equal_pivot_num =
      Partition(array, start + less_pivot_num, end, pivot + 1) + less_pivot_num;
  QuickSort(array, start, start + less_pivot_num);
  QuickSort(array, start + less_and_equal_pivot_num, end);
}

int main() {
  int array_len;
  std::cin >> array_len;
  std::vector<int> array(array_len);
  for (int i = 0; i < array_len; ++i) {
    std::cin >> array[i];
  }

  QuickSort(array, 0, array_len);

  for (int elem : array) {
    std::cout << elem << ' ';
  }
}
