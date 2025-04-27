/*N (<= 10^5) запросов к очереди:

  «+ i» — добавить число в конец оцереди;
  «* i» — добавить число в середину оцереди;
  «-» — удалить первое в очереди число, вывести его*/

#include <deque>
#include <iostream>
#include <queue>
#include <vector>

class GoblinQueue {
 public:
  GoblinQueue() {}

  size_t PopFront();

  void PushBack(size_t element);

  void PushMid(size_t element);

 private:
  std::queue<size_t> first_half_;
  std::deque<size_t> second_half_;

  void BalanceHalfs() {
    if (first_half_.size() < second_half_.size()) {
      first_half_.push(second_half_.front());
      second_half_.pop_front();
      return;
    }
    if (first_half_.size() > second_half_.size() + 1) {
      second_half_.push_front(first_half_.back());
      first_half_.pop();
    }
  }
};

size_t GoblinQueue::PopFront() {
  size_t front = first_half_.front();
  first_half_.pop();
  BalanceHalfs();
  return front;
}

void GoblinQueue::PushBack(size_t element) {
  second_half_.push_back(element);
  BalanceHalfs();
}

void GoblinQueue::PushMid(size_t element) {
  second_half_.push_front(element);
  BalanceHalfs();
}

int main() {
  size_t number;
  std::cin >> number;

  GoblinQueue goblins_queue;

  for (size_t i = 0; i < number; ++i) {
    char type;
    std::cin >> type;

    if (type == '-') {
      std::cout << goblins_queue.PopFront() << '\n';
      continue;
    }

    size_t id;
    std::cin >> id;

    if (type == '+') {
      goblins_queue.PushBack(id);
      continue;
    }

    goblins_queue.PushMid(id);
  }
}
