/*C помощью быстрой сортировки отсортировать массив.

  Написан детерминированный алгоритм (O(nlogn))*/

#include <iostream>
#include <vector>

int MedianShortArray(std::vector<int>& array) {  // длина массива - от 1 до 4
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

size_t Partition(std::vector<int>& array, size_t start, size_t end, int pivot) {
  size_t less_pivot = start;
  size_t more_pivot = end - 1;
  while (less_pivot < end && array[less_pivot] < pivot) {
    if (less_pivot == 0) {
      break;
    }
    ++less_pivot;
  }
  while (more_pivot >= start && array[more_pivot] >= pivot) {
    if (more_pivot == 0) {
      break;
    }
    --more_pivot;
  }
  while (less_pivot < more_pivot) {
    std::swap(array[less_pivot], array[more_pivot]);
    while (less_pivot < end && array[less_pivot] < pivot) {
      ++less_pivot;
    }
    while (more_pivot >= start && array[more_pivot] >= pivot) {
      if (more_pivot == 0) {
        break;
      }
      --more_pivot;
    }
  }
  return more_pivot + 1 - start;
}

int QuickSelect(std::vector<int>& array, size_t start, size_t end,
                size_t order_statistic_num, std::vector<int>& slice) {
  size_t lenght = end - start;
  if (lenght == 1) {
    return array[start];
  }

  const size_t kSliceLenght = 5;
  size_t medians_lenght = lenght / kSliceLenght;
  size_t last_slice_lenght = lenght % kSliceLenght;
  if (last_slice_lenght != 0) {
    ++medians_lenght;
  }
  std::vector<int> medians(medians_lenght);
  for (size_t i = 0; i < lenght / kSliceLenght; ++i) {
    for (size_t j = 0; j < kSliceLenght; ++j) {
      slice[j] = array[start + i * kSliceLenght + j];
    }
    medians[i] = MedianOf5(slice);
  }
  if (last_slice_lenght != 0) {
    std::vector<int> last_slice(last_slice_lenght);
    for (size_t i = 0; i < last_slice_lenght; ++i) {
      last_slice[i] = array[end - last_slice_lenght + i];
    }
    medians[medians_lenght - 1] = MedianShortArray(last_slice);
  }

  int pivot;
  if (medians_lenght < 5) {
    pivot = MedianShortArray(medians);
  } else if (medians_lenght == 5) {
    pivot = MedianOf5(medians);
  } else {
    pivot = QuickSelect(medians, 0, medians_lenght, (medians_lenght + 1) / 2,
                        slice);
  }
  size_t less_pivot_num = Partition(array, start, end, pivot);
  if (order_statistic_num <= less_pivot_num) {
    return QuickSelect(array, start, start + less_pivot_num,
                       order_statistic_num, slice);
  }
  size_t less_and_equal_pivot_num =
      Partition(array, start + less_pivot_num, end, pivot + 1) + less_pivot_num;
  if (order_statistic_num <= less_and_equal_pivot_num) {
    return pivot;
  }
  return QuickSelect(array, start + less_and_equal_pivot_num, end,
                     order_statistic_num - less_and_equal_pivot_num, slice);
}

void QuickSort(std::vector<int>& array, size_t start, size_t end) {
  size_t lenght = end - start;
  if (lenght <= 1) {
    return;
  }
  const size_t kSliceLenght = 5;
  std::vector<int> slice(kSliceLenght);
  int pivot = QuickSelect(array, start, end, (lenght + 1) / 2, slice);
  size_t less_pivot_num = Partition(array, start, end, pivot);
  size_t less_and_equal_pivot_num =
      Partition(array, start + less_pivot_num, end, pivot + 1) + less_pivot_num;
  QuickSort(array, start, start + less_pivot_num);
  QuickSort(array, start + less_and_equal_pivot_num, end);
}

int main() {
  size_t array_lenght;
  std::cin >> array_lenght;
  std::vector<int> array(array_lenght);
  for (size_t i = 0; i < array_lenght; ++i) {
    std::cin >> array[i];
  }

  QuickSort(array, 0, array_lenght);

  for (int element : array) {
    std::cout << element << ' ';
  }
}
