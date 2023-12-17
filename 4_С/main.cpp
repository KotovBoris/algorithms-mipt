// Даны пары строк
// Запрос: вывести пару данной строки

#include <iostream>
#include <string>
#include <vector>

class AVLTree {
 public:
  AVLTree(size_t max_size) { nodes_.reserve(max_size); }

  void Insert(std::string key, std::string value);

  std::string Find(std::string key);

 private:
  struct Node {
    std::string key;
    std::string value;
    size_t height = 1;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
  };
  std::vector<Node> nodes_;
  Node* root_ = nullptr;

  size_t static Height(Node* pointer);

  void static UpdateHeight(Node* node);

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

void AVLTree::Insert(std::string key, std::string value) {
  Node new_node{key, value};
  if (nodes_.empty()) {
    nodes_.push_back(new_node);
    root_ = nodes_.data();
    return;
  }

  Node* current = root_;
  while (true) {
    if (current->key == key) {
      return;
    }

    if (key < current->key) {
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

std::string AVLTree::Find(std::string key) {
  Node* current = root_;
  while (current != nullptr) {
    if (key == current->key) {
      return current->value;
    }
    if (key < current->key) {
      current = current->left;
      continue;
    }
    current = current->right;
  }
  return "";
}

size_t AVLTree::Height(Node* pointer) {
  return pointer != nullptr ? pointer->height : 0;
}

void AVLTree::UpdateHeight(Node* node) {
  node->height = std::max(Height(node->left), Height(node->right)) + 1;
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
  Balance(node->parent);
}

void AVLTree::HangNode(Node& parent, Node& son) {
  son.parent = &parent;
  nodes_.push_back(son);

  if (son.key < parent.key) {
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

  size_t data_size;
  std::cin >> data_size;

  AVLTree names(data_size);
  AVLTree cars(data_size);

  for (size_t i = 0; i < data_size; ++i) {
    std::string name;
    std::string car;
    std::cin >> name >> car;

    names.Insert(car, name);
    cars.Insert(name, car);
  }

  size_t requests_number;
  std::cin >> requests_number;

  for (size_t i = 0; i < requests_number; ++i) {
    std::string name_or_car;
    std::cin >> name_or_car;

    std::cout << names.Find(name_or_car) + cars.Find(name_or_car) << '\n';
  }
}
