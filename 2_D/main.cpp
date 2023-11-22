// Поддерживать минимум на очереди, запросов <= 2*10^5 

#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

void AddToMin(std::deque<size_t>& min, size_t iq) {
  while (!min.empty() && min.back() > iq) {
    min.pop_back();
  }
  min.push_back(iq);
}

void UpdateMin(std::deque<size_t>& min, size_t popped) {
  if (min.front() == popped) {
    min.pop_front();
  }
}

void PrintOk() { std::cout << "ok\n"; }

int main() {
  size_t number;
  std::cin >> number;

  std::queue<size_t> hat;
  std::deque<size_t> min;
  for (size_t i = 0; i < number; ++i) {
    std::string type;
    std::cin >> type;
    if (type == "enqueue") {
      size_t iq;
      std::cin >> iq;
      AddToMin(min, iq);
      hat.push(iq);
      PrintOk();
    }
    if (type == "clear") {
      hat = {};
      min.clear();
      PrintOk();
    } else if (type == "size") {
      std::cout << hat.size() << '\n';
    } else {
      if (hat.empty()) {
        std::cout << "error\n";
        continue;
      }
      if (type == "min") {
        std::cout << min[0] << '\n';
      }
      if (type == "dequeue") {
        std::cout << hat.front() << '\n';
        UpdateMin(min, hat.front());
        hat.pop();
      }
      if (type == "front") {
        std::cout << hat.front() << '\n';
      }
    }
  }
}
