#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

const size_t cAlphabetSize = 26;

const char cFirstLetter = 'a';
const char cLastLetter = 'z';

struct Node {
  size_t parent;
  char letter;
  size_t depth;
  std::vector<int> children;
  std::vector<size_t> go;
  size_t link;
  size_t term_link;
  bool terminal;

  Node(size_t parent = 0, char letter = ' ', size_t depth = 0)
      : parent(parent),
        letter(letter),
        depth(depth),
        children(cAlphabetSize, -1),
        go(cAlphabetSize, 0),
        link(0),
        term_link(0),
        terminal(false) {}
};

class Trie {
 private:
  std::vector<Node> nodes_;

 public:
  size_t static Root() { return 0; }

  Trie() { nodes_.push_back(Node()); }

  size_t AddWord(const std::string& word) { return AddSuffix(Root(), word, 0); }

  void AhoCorasickPreparation() {
    std::queue<size_t> bfs_queue;

    for (char letter = cFirstLetter; letter <= cLastLetter; ++letter) {
      int child = nodes_[Root()].children[letter - cFirstLetter];
      if (child == -1) {
        continue;
      }

      bfs_queue.push(child);
      nodes_[Root()].go[letter - cFirstLetter] = child;
    }

    while (!bfs_queue.empty()) {
      size_t index = bfs_queue.front();
      bfs_queue.pop();
      Node& current = nodes_[index];
      Node& parent = nodes_[current.parent];

      current.link =
          current.parent == 0
              ? 0
              : nodes_[parent.link].go[current.letter - cFirstLetter];

      for (char letter = cFirstLetter; letter <= cLastLetter; ++letter) {
        if (current.children[letter - cFirstLetter] == -1) {
          current.go[letter - cFirstLetter] =
              nodes_[current.link].go[letter - cFirstLetter];
          continue;
        }

        current.go[letter - cFirstLetter] =
            current.children[letter - cFirstLetter];
        bfs_queue.push(current.children[letter - cFirstLetter]);
      }

      current.term_link = current.link;
      if (!nodes_[current.link].terminal) {
        current.term_link = nodes_[current.link].term_link;
      }
    }
  }

  size_t Go(size_t node, char symbol) const {
    return nodes_[node].go[symbol - cFirstLetter];
  }

  size_t Link(size_t node) const { return nodes_[node].link; }

  size_t TermLink(size_t node) const { return nodes_[node].term_link; }

  size_t Depth(size_t node) const { return nodes_[node].depth; }

 private:
  void AddNode(size_t parent, char letter) {
    nodes_.push_back(Node(parent, letter, nodes_[parent].depth + 1));
    size_t new_node = nodes_.size() - 1;
    nodes_[parent].children[letter - cFirstLetter] = new_node;
  }

  size_t AddSuffix(size_t start_node, const std::string& word,
                   size_t suffix_start) {
    if (suffix_start == word.size()) {
      nodes_[start_node].terminal = true;
      return start_node;
    }

    char letter = word[suffix_start];
    int& child = nodes_[start_node].children[letter - cFirstLetter];
    if (child == -1) {
      AddNode(start_node, letter);
    }

    return AddSuffix(child, word, suffix_start + 1);
  }
};

std::vector<std::vector<size_t>> AhoCorasick(
    const std::string& text, const Trie& trie,
    std::unordered_map<size_t, size_t> indexes, std::vector<int> to_copy,
    size_t words_number) {
  std::vector<std::vector<size_t>> occurrences(words_number);

  size_t current_node = Trie::Root();
  for (size_t i = 0; i < text.size(); ++i) {
    current_node = trie.Go(current_node, text[i]);

    size_t word = current_node;
    if (indexes.contains(word)) {
      occurrences[indexes[word]].push_back(i - trie.Depth(word) + 2);
    }

    while (trie.TermLink(word) != 0) {
      word = trie.TermLink(word);
      occurrences[indexes[word]].push_back(i - trie.Depth(word) + 2);
    }
  }

  for (int i = static_cast<int>(occurrences.size() - 1); i >= 0; --i) {
    if (to_copy[i] != -1) {
      occurrences[i] = occurrences[to_copy[i]];
    }
  }

  return occurrences;
}

int main() {
  std::string text;
  std::cin >> text;

  size_t words_number;
  std::cin >> words_number;

  Trie trie;
  std::unordered_map<size_t, size_t> indexes;
  std::vector<int> to_copy(words_number, -1);
  for (size_t i = 0; i < words_number; ++i) {
    std::string word;
    std::cin >> word;

    size_t node_index = trie.AddWord(word);
    if (indexes.contains(node_index)) {
      to_copy[indexes[trie.AddWord(word)]] = i;
    }

    indexes[trie.AddWord(word)] = i;
  }

  trie.AhoCorasickPreparation();

  std::vector<std::vector<size_t>> occurrences =
      AhoCorasick(text, trie, indexes, to_copy, words_number);

  for (size_t i = 0; i < occurrences.size(); ++i) {
    std::cout << occurrences[i].size() << ' ';
    for (auto elem : occurrences[i]) {
      std::cout << elem << ' ';
    }
    std::cout << std::endl;
  }
}
