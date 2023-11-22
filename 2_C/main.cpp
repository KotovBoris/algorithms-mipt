/*N (<= 10^5) запросов к очереди:

  «+ i» — добавить число в конец оцереди;
  «* i» — добавить число в середину оцереди;
  «-» — удалить первое в очереди число, вывести его*/

#include <deque>
#include <iostream>
#include <vector>

void Balance(std::deque<size_t>& first_half, std::deque<size_t>& second_half) {
  if (first_half.size() < second_half.size()) {
    first_half.push_back(second_half.front());
    second_half.pop_front();
    return;
  }
  if (first_half.size() > second_half.size() + 1) {
    second_half.push_front(first_half.back());
    first_half.pop_back();
  }
}

int main() {
  size_t number;
  std::cin >> number;

  std::deque<size_t> first_half;
  std::deque<size_t> second_half;
  for (size_t i = 0; i < number; ++i) {
    char type;
    std::cin >> type;
    if (type == '-') {
      std::cout << first_half.front() << '\n';
      first_half.pop_front();
    } else {
      size_t id;
      std::cin >> id;
      if (type == '+') {
        second_half.push_back(id);
      } else {
        first_half.push_back(id);
      }
    }
    Balance(first_half, second_half);
  }
}
