#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

const size_t cAlphabetSize = 10;

struct Node {
  size_t depth;
  size_t count;
  std::vector<int> children;

  Node(size_t depth = 0)
      : depth(depth), count(0), children(cAlphabetSize, -1) {}
};

class Trie {
 private:
  std::vector<Node> nodes_;

 public:
  size_t static Root() { return 0; }
  Trie() { nodes_.push_back(Node()); }

  void AddWord(const std::string& word) {
    size_t curr = Root();
    ++nodes_[curr].count;

    for (size_t i = 0; i < word.size(); ++i) {
      for (size_t from_start = 0; from_start < 2; ++from_start) {
        char letter = (from_start == 0) ? word[i] : word[word.size() - i - 1];
        int& child = nodes_[curr].children[letter - '0'];
        if (child == -1) {
          AddNode(curr, letter);
        }

        curr = child;
        ++nodes_[curr].count;
      }
    }
  }

  std::vector<size_t> CountGroup(size_t group_size, size_t max_depth) {
    std::vector<size_t> groups(max_depth * 2 + 1, 0);

    std::queue<size_t> bfs_queue;
    bfs_queue.push(Root());

    while (!bfs_queue.empty()) {
      size_t index = bfs_queue.front();
      bfs_queue.pop();
      Node& current = nodes_[index];

      if (current.count >= group_size) {
        ++groups[current.depth];
      }

      for (char letter = '0'; letter <= '9'; ++letter) {
        if (current.children[letter - '0'] == -1) {
          continue;
        }

        bfs_queue.push(current.children[letter - '0']);
      }
    }

    return groups;
  }

 private:
  void AddNode(size_t parent, char letter) {
    nodes_.push_back(Node(nodes_[parent].depth + 1));
    size_t new_node = nodes_.size() - 1;
    nodes_[parent].children[letter - '0'] = new_node;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  size_t id_number;
  size_t group_size;
  std::cin >> id_number >> group_size;

  Trie trie;

  size_t max_len = 0;
  for (size_t i = 0; i < id_number; ++i) {
    std::string id;
    std::cin >> id;

    trie.AddWord(id);
    max_len = std::max(max_len, id.length());
  }

  std::vector<size_t> groups;
  groups = trie.CountGroup(group_size, max_len);

  size_t queries_num;
  std::cin >> queries_num;

  for (size_t i = 0; i < queries_num; ++i) {
    size_t query;
    std::cin >> query;
    std::cout << groups[query * 2] << '\n';
  }
}
