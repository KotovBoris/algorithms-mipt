// реализовать кучу на минимуме, изменять число, вставленное на i-ом запросе
// в любой момент в куче <= 10^5 элементов, запросов <= 10^6

#include <iostream>
#include <string>
#include <vector>

void SiftDown(std::vector<long long>& heap, size_t index,
              std::vector<size_t>& index_by_id,
              std::vector<size_t>& id_by_index) {
  while (index * 2 + 1 < heap.size()) {
    size_t right_child = index * 2 + 1;
    size_t left_child = index * 2 + 2;
    long long next = -1;

    if ((left_child >= heap.size() || heap[right_child] <= heap[left_child]) &&
        heap[right_child] < heap[index]) {
      next = right_child;
    } else if (left_child < heap.size() && heap[left_child] < heap[index]) {
      next = left_child;
    }
    if (next == -1) {
      return;
    }
    std::swap(heap[next], heap[index]);
    std::swap(index_by_id[id_by_index[next]], index_by_id[id_by_index[index]]);
    std::swap(id_by_index[next], id_by_index[index]);
    index = next;
  }
}

void SiftUp(std::vector<long long>& heap, size_t index,
            std::vector<size_t>& index_by_id,
            std::vector<size_t>& id_by_index) {
  size_t parent = (index - 1) / 2;
  while (index > 0 && heap[parent] > heap[index]) {
    std::swap(heap[parent], heap[index]);
    std::swap(index_by_id[id_by_index[parent]],
              index_by_id[id_by_index[index]]);
    std::swap(id_by_index[parent], id_by_index[index]);
    index = parent;
    parent = (index - 1) / 2;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t request_num;
  std::cin >> request_num;
  std::vector<size_t> index_by_id(request_num);
  std::vector<size_t> id_by_index;
  std::vector<long long> heap;

  for (size_t i = 0; i < request_num; ++i) {
    std::string type;
    std::cin >> type;
    if (type == "insert") {
      long long new_element;
      std::cin >> new_element;
      heap.push_back(new_element);
      id_by_index.push_back(i);
      index_by_id[i] = heap.size() - 1;
      SiftUp(heap, heap.size() - 1, index_by_id, id_by_index);
    }
    if (type == "getMin") {
      std::cout << heap[0] << '\n';
    }
    if (type == "extractMin") {
      index_by_id[id_by_index[heap.size() - 1]] = 0;
      id_by_index[0] = id_by_index[heap.size() - 1];
      id_by_index.resize(id_by_index.size() - 1);
      heap[0] = heap[heap.size() - 1];
      heap.resize(heap.size() - 1);
      if (heap.empty()) {
        continue;
      }
      SiftDown(heap, 0, index_by_id, id_by_index);
    }
    if (type == "decreaseKey") {
      size_t id;
      size_t shift;
      std::cin >> id >> shift;
      id -= 1;
      heap[index_by_id[id]] -= shift;
      SiftUp(heap, index_by_id[id], index_by_id, id_by_index);
    }
  }
}
