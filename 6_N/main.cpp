//  Найти минимальное "расстояние" между полученным сообщением и строкой,
//  полученной из отправленного (есть ограничение на кол-во изменений)

//  "расстояние" между строками - кол-во позиций,
//   на которых стоят разные символы

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

int MinDifferences(std::string sent, std::string recieved, size_t changes) {
  size_t sent_length = sent.size();
  size_t recieved_length = recieved.size();

  sent.push_back('1');  // чтобы не выходить за края
  recieved.push_back('2');

  const size_t kInfinity = std::numeric_limits<int>::max() / 2;

  std::vector<std::vector<int>> to_fill(
      sent_length + 2, std::vector<int>(recieved_length + 2, kInfinity));
  std::vector<std::vector<std::vector<int>>> dp(changes + 2, to_fill);
  //  dp[i][j][k] - ответ на задачу при i изменениниях,
  //  отправленной строке sent[:j], полученной - recieved[:k]

  //  База - 0 изменений на одинаковых перфиксах => 0:
  dp[0][0][0] = 0;
  for (size_t i = 1; i <= std::min(sent_length, recieved_length); ++i) {
    if (sent[i - 1] == recieved[i - 1]) {
      dp[0][i][i] = 0;
    } else {
      break;
    }
  }

  for (size_t i = 0; i <= changes; ++i) {
    for (size_t j = 0; j <= sent_length; ++j) {
      for (size_t k = 0; k <= recieved_length; ++k) {
        dp[i + 1][j + 1][k] = std::min(dp[i + 1][j + 1][k], dp[i][j][k]);

        dp[i + 1][j][k + 1] = std::min(dp[i + 1][j][k + 1], dp[i][j][k]);

        dp[i + 1][j + 1][k + 1] =
            std::min(dp[i + 1][j + 1][k + 1], dp[i][j][k]);
        dp[i][j + 1][k + 1] = std::min(dp[i][j + 1][k + 1], dp[i][j][k] + 1);

        if (sent[j] == recieved[k]) {
          dp[i][j + 1][k + 1] = std::min(dp[i][j + 1][k + 1], dp[i][j][k]);
        }
      }
    }
  }

  if (dp[changes][sent_length][recieved_length] == kInfinity) {
    return -1;
  }

  return dp[changes][sent_length][recieved_length];
}

int main() {
  std::string sent;
  std::string recieved;
  size_t changes;
  std::cin >> sent >> recieved >> changes;

  std::cout << MinDifferences(sent, recieved, changes);
}
