//  Найти минимальное решение пятнашек

#include <algorithm>
#include <bitset>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

const int cBoardSize = 4;
const int cBitsPerCell = 4;
const int cBitsetSize = cBoardSize * cBoardSize * cBitsPerCell;
const int cMaxDepth = 80;
const std::vector<std::pair<int, int>> cDirections = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const std::vector<char> cDirectionLabels = {'D', 'L', 'U', 'R'};

void SwapBits(std::bitset<cBitsetSize>& bits, int from, int to, int count);

class State {
 public:
  static State FakeState();
  State(const std::vector<std::vector<int>>& table);
  int GetValue(int index) const;
  std::vector<State> GetNeighbors() const;
  int Heuristics() const;  //  манхэттенское расстояние, линейные конфликты
  bool operator==(const State& other) const;
  std::bitset<cBitsetSize> GetBitset() const;
  int FindValue(int val) const;

 private:
  std::bitset<cBitsetSize> state_;
  int ManhattanDistance() const;
  int LinearConflicts() const;
};

struct DistanceUpperBound {
  int distance;
  State state;
  bool operator<(const DistanceUpperBound& other) const;
};

struct MinDistance {
  int distance;
  char move;
  MinDistance* prev_state = nullptr;
};

namespace std {
template <>
struct hash<State> {
  std::size_t operator()(const State& obj) const;
};
}  // namespace std

class Puzzle {
 public:
  static State FinalState();
  static std::string FindShortestSolution(const State& start);

 private:
  static bool IsSolvable(const State& state);
};

void SwapBits(std::bitset<cBitsetSize>& bits, int from, int to, int count) {
  for (int i = 0; i < count; ++i) {
    bool temp = bits[from + i];
    bits[from + i] = bits[to + i];
    bits[to + i] = temp;
  }
}

State State::FakeState() {
  std::vector<int> zeros(cBoardSize, 0);
  return State(std::vector<std::vector<int>>(cBoardSize, zeros));
}

State::State(const std::vector<std::vector<int>>& table) {
  for (int i = 0; i < cBoardSize; ++i) {
    for (int j = 0; j < cBoardSize; ++j) {
      int val = table[i][j];

      for (int k = 0; k < cBitsPerCell; ++k) {
        state_[i * cBoardSize * cBitsPerCell + j * cBitsPerCell + k] =
            (((val >> k) & 1) != 0);
      }
    }
  }
}

int State::GetValue(int index) const {
  int value = 0;
  for (int k = 0; k < cBitsPerCell; ++k) {
    value |= static_cast<int>(state_[index * cBitsPerCell + k]) << k;
  }
  return value;
}

std::vector<State> State::GetNeighbors() const {
  std::vector<State> neighbors;
  int zero_pos = FindValue(0);
  int x = zero_pos / cBoardSize;
  int y = zero_pos % cBoardSize;

  for (size_t k = 0; k < cDirections.size(); ++k) {
    int new_x = x + cDirections[k].first;
    int new_y = y + cDirections[k].second;

    if (new_x >= 0 && new_x < cBoardSize && new_y >= 0 && new_y < cBoardSize) {
      State neighbor = *this;
      int new_zero_pos = new_x * cBoardSize + new_y;
      SwapBits(neighbor.state_, zero_pos * cBitsPerCell,
               new_zero_pos * cBitsPerCell, cBitsPerCell);
      neighbors.push_back(neighbor);
    } else {
      neighbors.push_back(FakeState());
    }
  }
  return neighbors;
}

int State::Heuristics() const {
  return ManhattanDistance() + LinearConflicts();
}

bool State::operator==(const State& other) const {
  return state_ == other.state_;
}

std::bitset<cBitsetSize> State::GetBitset() const { return state_; }

int State::FindValue(int val) const {
  for (size_t index = 0; index < cBoardSize * cBoardSize; ++index) {
    if (GetValue(index) == val) {
      return index;
    }
  }

  return -1;
}

int State::ManhattanDistance() const {
  int distance = 0;
  for (int pos = 0; pos < cBoardSize * cBoardSize; ++pos) {
    int val = GetValue(pos);
    if (val == 0) {
      continue;
    }

    int target_pos = val - 1;
    int target_x = target_pos / cBoardSize;
    int target_y = target_pos % cBoardSize;
    int x = pos / cBoardSize;
    int y = pos % cBoardSize;

    distance += std::abs(x - target_x) + std::abs(y - target_y);
  }
  return distance;
}

int State::LinearConflicts() const {
  int conflicts = 0;
  std::bitset<cBoardSize> row_conflicts;
  std::bitset<cBoardSize> col_conflicts;

  for (int pos = 0; pos < cBoardSize * cBoardSize; ++pos) {
    int val = GetValue(pos);
    if (val == 0) {
      continue;
    }

    int target_pos = val - 1;
    int target_x = target_pos / cBoardSize;
    int target_y = target_pos % cBoardSize;
    int x = pos / cBoardSize;
    int y = pos % cBoardSize;

    if (x == target_x && !row_conflicts[x]) {
      for (int j = y + 1; j < cBoardSize; ++j) {
        int other_val = GetValue(x * cBoardSize + j);
        if (other_val == 0) {
          continue;
        }
        int other_target_pos = other_val - 1;
        int other_target_x = other_target_pos / cBoardSize;
        if (other_target_x == x && other_val < val) {
          conflicts += 1;
          row_conflicts[x] = true;
          break;
        }
      }
    }

    if (y == target_y && !col_conflicts[y]) {
      for (int i = x + 1; i < cBoardSize; ++i) {
        int other_val = GetValue(i * cBoardSize + y);
        if (other_val == 0) {
          continue;
        }
        int other_target_pos = other_val - 1;
        int other_target_y = other_target_pos % cBoardSize;
        if (other_target_y == y && other_val < val) {
          conflicts += 1;
          col_conflicts[y] = true;
          break;
        }
      }
    }
  }
  return 2 * conflicts;
}

bool DistanceUpperBound::operator<(const DistanceUpperBound& other) const {
  return distance > other.distance;
}

std::size_t std::hash<State>::operator()(const State& obj) const {
  return std::hash<unsigned long long>()(obj.GetBitset().to_ullong());
}

State Puzzle::FinalState() {
  std::vector<std::vector<int>> table(cBoardSize, std::vector<int>(cBoardSize));

  for (size_t x = 0; x < cBoardSize; ++x) {
    for (size_t y = 0; y < cBoardSize; ++y) {
      table[x][y] = x * cBoardSize + y + 1;
    }
  }
  table[cBoardSize - 1][cBoardSize - 1] = 0;

  return State(table);
}

std::string Puzzle::FindShortestSolution(const State& start) {
  if (!IsSolvable(start)) {
    return "unsolvable";
  }

  std::unordered_map<State, MinDistance> hash_map;
  std::priority_queue<DistanceUpperBound> heap;

  State final_state = FinalState();

  hash_map[start].distance = start.Heuristics();
  heap.push({hash_map[start].distance, start});

  int curr_distance = 0;

  while (!heap.empty()) {
    State curr_state = heap.top().state;
    curr_distance = heap.top().distance;
    heap.pop();

    if (curr_state == final_state) {
      break;
    }

    if (curr_distance > hash_map[curr_state].distance) {
      continue;
    }

    std::vector<State> neighbors = curr_state.GetNeighbors();
    for (size_t i = 0; i < neighbors.size(); ++i) {
      State& neighbor = neighbors[i];

      if (neighbor == State::FakeState()) {
        continue;
      }

      int new_distance =
          curr_distance - curr_state.Heuristics() + neighbor.Heuristics() + 1;

      if (new_distance <= cMaxDepth &&
          (hash_map.find(neighbor) == hash_map.end() ||
           new_distance < hash_map[neighbor].distance)) {
        hash_map[neighbor].move = cDirectionLabels[i];
        hash_map[neighbor].prev_state = &hash_map[curr_state];
        hash_map[neighbor].distance = new_distance;
        heap.push({new_distance, neighbor});
      }
    }
  }

  std::string path;
  MinDistance* curr_node = &hash_map[final_state];
  while (curr_node->prev_state != nullptr) {
    char direction = curr_node->move;
    path.push_back(direction);
    curr_node = curr_node->prev_state;
  }
  std::reverse(path.begin(), path.end());

  return path;
}

bool Puzzle::IsSolvable(const State& state) {
  int inversions = 0;
  std::vector<int> values;

  for (int index = 0; index < cBoardSize * cBoardSize; ++index) {
    int val = state.GetValue(index);
    if (val != 0) {
      values.push_back(val);
    }
  }

  for (size_t i = 0; i < values.size(); ++i) {
    for (size_t j = i + 1; j < values.size(); ++j) {
      if (values[i] > values[j]) {
        ++inversions;
      }
    }
  }

  int zero_x = state.FindValue(0) / cBoardSize;
  return ((zero_x + inversions) % 2 == 1);
}

int main() {
  std::vector<std::vector<int>> table;
  for (size_t i = 0; i < cBoardSize; ++i) {
    table.push_back({});
    for (size_t j = 0; j < cBoardSize; ++j) {
      size_t value;
      std::cin >> value;
      table[i].push_back(value);
    }
  }

  std::string turns = Puzzle::FindShortestSolution(State(table));
  if (turns == "unsolvable") {
    std::cout << -1;
    return 0;
  }

  std::cout << turns.size() << '\n' << turns;
}
