// Поддерживать минимум на очереди, запросов <= 2*10^5

#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

class QueueWithMin {
 public:
  void Push(size_t element);

  void Clear();

  size_t Size() { return data_.size(); };

  size_t Front() { return data_.front(); }

  size_t Min() { return min_[0]; }

  size_t PopFront();

 private:
  std::queue<size_t> data_;
  std::deque<size_t> min_;
};

void QueueWithMin::Push(size_t element) {
  while (!min_.empty() && min_.back() > element) {
    min_.pop_back();
  }
  min_.push_back(element);

  data_.push(element);
}

void QueueWithMin::Clear() {
  data_ = {};
  min_.clear();
}

size_t QueueWithMin::PopFront() {
  size_t popped = Front();
  if (min_.front() == popped) {
    min_.pop_front();
  }
  data_.pop();

  return popped;
}

int main() {
  size_t number;
  std::cin >> number;

  QueueWithMin hat;

  for (size_t i = 0; i < number; ++i) {
    std::string type;
    std::cin >> type;

    if (type == "enqueue") {
      size_t iq;
      std::cin >> iq;
      hat.Push(iq);
      std::cout << "ok\n";
      continue;
    }

    if (type == "clear") {
      hat.Clear();
      std::cout << "ok\n";
      continue;
    }

    if (type == "size") {
      std::cout << hat.Size() << '\n';
      continue;
    }

    if (hat.Size() == 0) {
      std::cout << "error\n";
      continue;
    }

    if (type == "min") {
      std::cout << hat.Min() << '\n';
      continue;
    }

    if (type == "dequeue") {
      std::cout << hat.PopFront() << '\n';
      continue;
    }

    if (type == "front") {
      std::cout << hat.Front() << '\n';
    }
  }
}
