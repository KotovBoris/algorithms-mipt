/* Реализовать AVL-дерево с операциями:
   + i: Добавить i
   ? i: Минимальный элемент >= i */

#include <iostream>
#include <vector>

class AVLTree {
 public:
  AVLTree(size_t max_size) { nodes_.reserve(max_size); }

  void Insert(long long element);

  long long LowerBound(long long bound);

 private:
  struct Node {
    long long value;
    long long max = value;
    size_t height = 1;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
  };
  std::vector<Node> nodes_;
  Node* root_ = nullptr;

  size_t static Height(Node* pointer);

  long long static Max(Node* pointer);

  void static UpdateHeight(Node* node);

  void static UpdateMax(Node* node);

  long long static Delta(Node* node);

  void static Rotate(Node*& root, Node* Node::*field_new,
                     Node* Node::*other_field);

  void static RotateLeft(Node*& root);

  void static RotateRight(Node*& root);

  void PickRotate(Node* node, Node* Node::*field_new, Node* Node::*other_field);

  void PickRotateRight(Node* node);

  void PickRotateLeft(Node* node);

  void Balance(Node* node);

  void HangNode(Node& parent, Node& son);
};

void AVLTree::Insert(long long element) {
  Node new_node{element};
  if (nodes_.empty()) {
    nodes_.push_back(new_node);
    root_ = nodes_.data();
    return;
  }

  Node* current = root_;
  while (true) {
    if (current->value == element) {
      return;
    }

    if (element < current->value) {
      if (current->left == nullptr) {
        HangNode(*current, new_node);
        return;
      }

      current = current->left;
      continue;
    }

    if (current->right == nullptr) {
      HangNode(*current, new_node);
      return;
    }

    current = current->right;
  }
}

long long AVLTree::LowerBound(long long bound) {
  if (bound > Max(root_)) {
    return -1;
  }

  Node* current = root_;
  while (true) {
    if (bound <= Max(current->left)) {
      current = current->left;
      continue;
    }
    if (bound <= current->value) {
      return current->value;
    }
    current = current->right;
  }
}

size_t AVLTree::Height(Node* pointer) {
  return pointer != nullptr ? pointer->height : 0;
}

long long AVLTree::Max(Node* pointer) {
  return pointer != nullptr ? pointer->max : -1;
}

void AVLTree::UpdateHeight(Node* node) {
  node->height = std::max(Height(node->left), Height(node->right)) + 1;
}

void AVLTree::UpdateMax(Node* node) {
  long long subtrees_max = std::max(Max(node->left), Max(node->right));
  node->max = std::max(node->value, subtrees_max);
}

long long AVLTree::Delta(Node* node) {
  long long left = Height(node->left);
  long long right = Height(node->right);

  return left - right;
}

void AVLTree::Rotate(Node*& root, Node* Node::*field_new,
                     Node* Node::*other_field) {
  Node* old_root = root;
  root = root->*field_new;
  Node* to_rehang = root->*other_field;
  if (to_rehang != nullptr) {
    to_rehang->parent = old_root;
  }
  old_root->*field_new = to_rehang;
  root->*other_field = old_root;

  root->parent = old_root->parent;
  old_root->parent = root;
  UpdateHeight(old_root);
  UpdateHeight(root);
  UpdateMax(old_root);
  UpdateMax(root);
}

void AVLTree::RotateLeft(Node*& root) {
  Rotate(root, &Node::right, &Node::left);
}

void AVLTree::RotateRight(Node*& root) {
  Rotate(root, &Node::left, &Node::right);
}

void AVLTree::PickRotate(Node* node, Node* Node::*field_new,
                         Node* Node::*other_field) {
  if (node->parent == nullptr) {
    Rotate(root_, field_new, other_field);
    return;
  }

  if (node->parent->left == node) {
    Rotate(node->parent->left, field_new, other_field);
    return;
  }
  if (node->parent->right == node) {
    Rotate(node->parent->right, field_new, other_field);
    return;
  }
}

void AVLTree::PickRotateRight(Node* node) {
  PickRotate(node, &Node::left, &Node::right);
}

void AVLTree::PickRotateLeft(Node* node) {
  PickRotate(node, &Node::right, &Node::left);
}

void AVLTree::Balance(Node* node) {
  if (node == nullptr) {
    return;
  }

  if (Delta(node) == -2) {
    if (Delta(node->right) == 1) {
      RotateRight(node->right);
    }

    PickRotateLeft(node);
  }

  if (Delta(node) == 2) {
    if (Delta(node->left) == -1) {
      RotateLeft(node->left);
    }

    PickRotateRight(node);
  }

  UpdateHeight(node);
  UpdateMax(node);
  Balance(node->parent);
}

void AVLTree::HangNode(Node& parent, Node& son) {
  son.parent = &parent;
  nodes_.push_back(son);

  if (son.value < parent.value) {
    parent.left = &nodes_.back();
  } else {
    parent.right = &nodes_.back();
  }

  Balance(&nodes_.back());
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  size_t requests_number;
  std::cin >> requests_number;

  AVLTree tree(requests_number);

  long long last_answer = 0;
  for (size_t i = 0; i < requests_number; ++i) {
    char type;
    size_t time;
    std::cin >> type >> time;

    const size_t kMod = 1000000000;
    switch (type) {
      case '+':
        tree.Insert((time + last_answer) % kMod);
        last_answer = 0;
        break;
      case '?':
        last_answer = tree.LowerBound(time);
        std::cout << last_answer << '\n';
    }
  }
}
