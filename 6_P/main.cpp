#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>

size_t Bit(size_t number, size_t position) {
  return static_cast<size_t>((number & (1 << position)) != 0U);
}

bool IsCompatible(const std::vector<std::vector<int>>& map, size_t profile,
                  size_t collumn) {
  for (size_t i = 0; i < map.size(); ++i) {
    if (map[i][collumn] >= 0 &&
        map[i][collumn] != static_cast<int>(Bit(profile, i))) {
      return false;
    }
  }

  return true;
}

std::vector<size_t> NextProfiles(size_t profile, size_t max_length) {
  std::vector<size_t> next_profiles;

  for (size_t first_bit : {0, 1}) {
    size_t next = first_bit;

    if (max_length >= 2) {
      size_t triangle_sum = first_bit + Bit(profile, 0) + Bit(profile, 1);
      if (triangle_sum == 0 || triangle_sum == 3) {
        continue;
      }
    }

    bool exist = true;

    for (size_t i = 1; i < max_length; ++i) {
      int new_bit =
          2 - Bit(next, i - 1) - Bit(profile, i - 1) - Bit(profile, i);
      if (new_bit < 0 || new_bit > 1) {
        exist = false;
        break;
      }

      next += new_bit << i;
    }

    if (exist) {
      next_profiles.push_back(next);
    }
  }

  return next_profiles;
}

size_t OptionsNumber(const std::vector<std::vector<int>>& map) {
  const size_t kMod = 1000000007;

  size_t length = map[0].size();
  size_t max_profile = 1 << map.size();

  std::vector<std::vector<size_t>> dp(length, std::vector<size_t>(max_profile));

  //  dp[i][j] - кол-во способов замостить карту n на i,
  //             если в последнем столбце - профиль j
  //  База: длина = 1 => если профиль совместим с картой, то 1, иначе - 0:
  for (size_t profile = 0; profile < max_profile; ++profile) {
    if (IsCompatible(map, profile, 0)) {
      dp[0][profile] = 1;
    }
  }

  for (size_t i = 0; i < length - 1; ++i) {
    for (size_t profile = 0; profile < max_profile; ++profile) {
      if (dp[i][profile] == 0) {
        continue;
      }

      for (size_t next : NextProfiles(profile, map.size())) {
        if (IsCompatible(map, next, i + 1)) {
          dp[i + 1][next] += dp[i][profile];
          dp[i + 1][next] %= kMod;
        }
      }
    }
  }

  return std::accumulate(dp[length - 1].begin(), dp[length - 1].end(), 0) %
         kMod;
}

int main() {
  size_t rows;
  size_t collumns;
  std::cin >> rows >> collumns;

  std::vector<std::vector<int>> map(rows, std::vector<int>(collumns));

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < collumns; ++j) {
      char symbol;
      std::cin >> symbol;

      if (symbol == '+') {
        map[i][j] = 1;
        continue;
      }
      if (symbol == '-') {
        map[i][j] = 0;
        continue;
      }

      map[i][j] = -1;
    }
  }

  std::cout << OptionsNumber(map);
}
