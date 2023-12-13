// реализовать кучу на минимуме, изменять число, вставленное на i-ом запросе
// в любой момент в куче <= 10^5 элементов, запросов <= 10^6

#include <iostream>
#include <string>
#include <vector>

class Heap {
 public:
  Heap(size_t request_num) : index_by_id_(request_num) {}

  void Insert(long long element, size_t id);

  void ExtractMin();

  void DecreaseKey(size_t id, size_t shift);

  long long GetMin() { return data_[0]; }

 private:
  std::vector<size_t> index_by_id_;
  std::vector<size_t> id_by_index_;
  std::vector<long long> data_;

  void SiftDown(size_t index) {
    while (index * 2 + 1 < data_.size()) {
      size_t right_child = index * 2 + 1;
      size_t left_child = index * 2 + 2;
      long long next = -1;
      if ((left_child >= data_.size() ||
           data_[right_child] <= data_[left_child]) &&
          data_[right_child] < data_[index]) {
        next = right_child;
      } else if (left_child < data_.size() &&
                 data_[left_child] < data_[index]) {
        next = left_child;
      }

      if (next == -1) {
        return;
      }

      Swap(next, index);
      index = next;
    }
  }

  void SiftUp(size_t index) {
    size_t parent = (index - 1) / 2;
    while (index > 0 && data_[parent] > data_[index]) {
      Swap(parent, index);
      index = parent;
      parent = (index - 1) / 2;
    }
  }

  void Swap(size_t index1, size_t index2) {
    std::swap(data_[index1], data_[index2]);
    std::swap(index_by_id_[id_by_index_[index1]],
              index_by_id_[id_by_index_[index2]]);
    std::swap(id_by_index_[index1], id_by_index_[index2]);
  }
};

void Heap::Insert(long long element, size_t id) {
  data_.push_back(element);
  id_by_index_.push_back(id);
  index_by_id_[id] = data_.size() - 1;
  SiftUp(data_.size() - 1);
}

void Heap::ExtractMin() {
  Swap(0, data_.size() - 1);
  id_by_index_.resize(id_by_index_.size() - 1);
  data_.resize(data_.size() - 1);

  if (data_.empty()) {
    return;
  }

  SiftDown(0);
}

void Heap::DecreaseKey(size_t id, size_t shift) {
  data_[index_by_id_[id]] -= shift;
  SiftUp(index_by_id_[id]);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t request_num;
  std::cin >> request_num;
  Heap heap(request_num);

  for (size_t i = 0; i < request_num; ++i) {
    std::string type;
    std::cin >> type;
    if (type == "insert") {
      long long new_element;
      std::cin >> new_element;
      heap.Insert(new_element, i);
      continue;
    }
    if (type == "getMin") {
      std::cout << heap.GetMin() << '\n';
      continue;
    }
    if (type == "extractMin") {
      heap.ExtractMin();
    }
    if (type == "decreaseKey") {
      size_t id;
      size_t shift;
      std::cin >> id >> shift;
      heap.DecreaseKey(id - 1, shift);
    }
  }
}
