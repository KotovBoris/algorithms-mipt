// Реализовать СД,  основанную на куче, на которой поддерживаются минимум и максимум
// Можно добавлять элементы, удалять минимум/максимум. Запросов <= 2*10^5

#include <iostream>
#include <string>
#include <vector>

bool Less(long long first, long long second) { return (first < second); }

bool More(long long first, long long second) { return (first > second); }

class Heap {
 public:
  // компаратор - условие кучи: Less => на min
  Heap(size_t request_num, bool (*comparator)(long long, long long))
      : size_(0),
        data_({}),
        comparator_(comparator),
        index_by_id_(std::vector<size_t>(request_num)),
        id_by_index_({}) {}
  void Insert(long long number, size_t id);
  size_t Size() const { return size_; }
  long long GetRoot() { return data_[0]; }
  void DeleteRoot();
  void Erase(size_t id);
  void Clear();
  size_t GetRootId() { return id_by_index_[0]; }

 private:
  void Swap(size_t index1, size_t index2);
  void SiftUp(size_t index);
  void SiftDown(size_t index);
  size_t size_;
  std::vector<long long> data_;
  bool (*comparator_)(long long, long long);
  std::vector<size_t> index_by_id_;
  std::vector<size_t> id_by_index_;
};

void Heap::Swap(size_t index1, size_t index2) {
  std::swap(data_[index1], data_[index2]);
  std::swap(index_by_id_[id_by_index_[index1]],
            index_by_id_[id_by_index_[index2]]);
  std::swap(id_by_index_[index1], id_by_index_[index2]);
}

void Heap::SiftUp(size_t index) {
  size_t parent = (index - 1) / 2;
  while (index > 0 && comparator_(data_[index], data_[parent])) {
    this->Swap(parent, index);
    index = parent;
    parent = (index - 1) / 2;
  }
}

void Heap::SiftDown(size_t index) {
  while (index * 2 + 1 < size_) {
    size_t right_child = index * 2 + 1;
    size_t left_child = index * 2 + 2;
    long long next = -1;

    if ((left_child >= size_ ||
         comparator_(data_[right_child], data_[left_child])) &&
        comparator_(data_[right_child], data_[index])) {
      next = right_child;
    } else if (left_child < size_ &&
               comparator_(data_[left_child], data_[index])) {
      next = left_child;
    }
    if (next == -1) {
      return;
    }
    this->Swap(next, index);
    index = next;
  }
}

void Heap::Insert(long long number, size_t id) {
  ++size_;
  data_.push_back(number);
  id_by_index_.push_back(id);
  index_by_id_[id] = size_ - 1;
  this->SiftUp(size_ - 1);
}

void Heap::DeleteRoot() {
  --size_;
  index_by_id_[id_by_index_[size_]] = 0;
  id_by_index_[0] = id_by_index_[size_];
  id_by_index_.resize(size_);
  data_[0] = data_[size_];
  data_.resize(size_);
  if (data_.empty()) {
    return;
  }
  this->SiftDown(0);
}

void Heap::Erase(size_t id) {
  --size_;
  size_t index = index_by_id_[id];
  index_by_id_[id_by_index_[size_]] = index;
  id_by_index_[index] = id_by_index_[size_];
  id_by_index_.resize(size_);
  data_[index] = data_[size_];
  data_.resize(size_);
  if (data_.empty()) {
    return;
  }
  this->SiftDown(index);
  this->SiftUp(index);
}

void Heap::Clear() {
  size_ = 0;
  data_ = {};
  id_by_index_ = {};
}

class TwoOrderedHeap {
 public:
  TwoOrderedHeap(size_t request_num)
      : heap_min_(Heap(request_num, Less)),
        heap_max_(Heap(request_num, More)) {}
  void Insert(long long number, size_t id);
  void DeleteMin();
  long long GetMin() { return heap_min_.GetRoot(); }
  void DeleteMax();
  long long GetMax() { return heap_max_.GetRoot(); }
  size_t Size() const { return heap_max_.Size(); }
  void Clear() {
    heap_max_.Clear();
    heap_min_.Clear();
  }

 private:
  Heap heap_min_;
  Heap heap_max_;
};

void TwoOrderedHeap::Insert(long long number, size_t id) {
  heap_max_.Insert(number, id);
  heap_min_.Insert(number, id);
}

void TwoOrderedHeap::DeleteMin() {
  size_t id_min = heap_min_.GetRootId();
  heap_min_.DeleteRoot();
  heap_max_.Erase(id_min);
}

void TwoOrderedHeap::DeleteMax() {
  size_t id_max = heap_max_.GetRootId();
  heap_max_.DeleteRoot();
  heap_min_.Erase(id_max);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t request_num;
  std::cin >> request_num;
  TwoOrderedHeap minimax(request_num);

  for (size_t i = 0; i < request_num; ++i) {
    std::string type;
    std::cin >> type;

    if (type == "insert") {
      long long new_element;
      std::cin >> new_element;
      minimax.Insert(new_element, i);
      std::cout << "ok\n";
      continue;
    }
    if (type == "size") {
      std::cout << minimax.Size() << '\n';
      continue;
    }
    if (type == "clear") {
      minimax.Clear();
      std::cout << "ok\n";
      continue;
    }

    if (minimax.Size() == 0) {
      std::cout << "error\n";
      continue;
    }

    if (type == "extract_min") {
      std::cout << minimax.GetMin() << '\n';
      minimax.DeleteMin();
    }
    if (type == "get_min") {
      std::cout << minimax.GetMin() << '\n';
    }
    if (type == "extract_max") {
      std::cout << minimax.GetMax() << '\n';
      minimax.DeleteMax();
    }
    if (type == "get_max") {
      std::cout << minimax.GetMax() << '\n';
    }
  }
}
