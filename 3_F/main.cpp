/*Дан массив чисел (длина <= 10^5), <= 10^5 запросов вида:
  сколько среди чисел, стоящих на позициях с start по end,
  по величине лежат в интервале от min до max*/

#include <algorithm>
#include <iostream>
#include <vector>

class SegmentTree {
 public:
  SegmentTree(const std::vector<size_t>& data);

  long long GetValue(size_t start, size_t end, size_t min, size_t max);

 private:
  struct Node {
    size_t start;
    size_t end;
    std::vector<size_t> value;
    Node* parent;
    Node* left;
    Node* right;

    Node Merge(const Node& second) const;

    void CalculateNode() { value = (left->Merge(*right)).value; }

    void UpdateParents() const;
  };

  std::vector<size_t> data_;
  std::vector<Node> nodes_;
  std::vector<Node*> leaves_;

  size_t CreateSon(size_t start, size_t end, Node* parent);

  void CreateSubtree(Node& node);

  long long MergeFundamental(size_t min, size_t max, size_t start, size_t end,
                             Node& node);
};

SegmentTree::SegmentTree(const std::vector<size_t>& data)
    : data_(data), leaves_(data.size()) {
  nodes_.reserve(4 * data.size());

  nodes_.push_back(Node{0, data_.size() - 1, {}, nullptr, nullptr, nullptr});
  CreateSubtree(nodes_[0]);
}

long long SegmentTree::GetValue(size_t start, size_t end, size_t min,
                                size_t max) {
  return MergeFundamental(min, max, start, end, nodes_[0]);
}

size_t SegmentTree::CreateSon(size_t start, size_t end, Node* parent) {
  size_t index = nodes_.size();
  nodes_.push_back(
      Node{start, end, std::vector<size_t>(), parent, nullptr, nullptr});
  CreateSubtree(nodes_[index]);
  return index;
}

void SegmentTree::CreateSubtree(Node& node) {
  if (node.start == node.end) {
    node.value = {data_[node.start]};
    leaves_[node.start] = &node;
    return;
  }
  size_t end_left = (node.start + node.end) / 2;
  size_t start_right = end_left + 1;

  size_t index_left = CreateSon(node.start, end_left, &node);
  node.left = &nodes_[index_left];

  size_t index_right = CreateSon(start_right, node.end, &node);
  node.right = &nodes_[index_right];

  node.CalculateNode();
}

long long SegmentTree::MergeFundamental(size_t min, size_t max, size_t start,
                                        size_t end, Node& node) {
  if (start == node.start && end == node.end) {
    const std::vector<size_t>& array = node.value;
    return std::upper_bound(array.begin(), array.end(), max) -
           std::lower_bound(array.begin(), array.end(), min);
  }
  if (node.left != nullptr && start <= node.left->end &&
      node.right != nullptr && end >= node.right->start) {
    size_t left_part = MergeFundamental(
        min, max, start, std::min(node.left->end, end), *node.left);
    size_t right_part = MergeFundamental(
        min, max, std::max(start, node.right->start), end, *node.right);
    return left_part + right_part;
  }
  if (node.left != nullptr && start <= node.left->end) {
    return MergeFundamental(min, max, start, std::min(node.left->end, end),
                            *node.left);
  }
  return MergeFundamental(min, max, std::max(start, node.right->start), end,
                          *node.right);
}

SegmentTree::Node SegmentTree::Node::Merge(const Node& second) const {
  Node copy{start,
            second.end,
            std::vector<size_t>(value.size() + second.value.size()),
            nullptr,
            nullptr,
            nullptr};
  size_t first_index = 0;
  size_t second_index = 0;
  while (first_index + second_index < copy.value.size()) {
    if (first_index >= value.size()) {
      copy.value[first_index + second_index] = second.value[second_index];
      ++second_index;
      continue;
    }
    if (second_index >= second.value.size()) {
      copy.value[first_index + second_index] = value[first_index];
      ++first_index;
      continue;
    }
    if (value[first_index] <= second.value[second_index]) {
      copy.value[first_index + second_index] = value[first_index];
      ++first_index;
      continue;
    }
    copy.value[first_index + second_index] = second.value[second_index];
    ++second_index;
  }
  return copy;
}

void SegmentTree::Node::UpdateParents() const {
  if (parent == nullptr) {
    return;
  }
  parent->CalculateNode();
  parent->UpdateParents();
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  size_t length;
  size_t request_number;
  std::cin >> length >> request_number;

  std::vector<size_t> data(length);
  for (size_t i = 0; i < length; i++) {
    std::cin >> data[i];
  }

  SegmentTree tree(data);

  for (size_t i = 0; i < request_number; i++) {
    size_t start;
    size_t end;
    size_t min;
    size_t max;
    std::cin >> start >> end >> min >> max;
    std::cout << tree.GetValue(start - 1, end - 1, min, max) << '\n';
  }
}
