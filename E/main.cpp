#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

//  n >= 2
class Z {
 public:
  size_t size;

 private:
  long long value_;

  void Normalize() {
    if (value_ < 0) {
      value_ = -value_ % size;
      value_ *= -1;
      value_ += size;
    }
    value_ %= size;
  }

 public:
  explicit Z(size_t size, long long value = 0) : size(size), value_(value) {
    Normalize();
  }

  Z(const Z& other) : size(other.size), value_(other.value_) {}

  Z& operator=(const Z& other) {
    value_ = other.value_;
    size = other.size;

    return *this;
  }

  Z& operator+=(const Z& other) {
    value_ += other.value_;
    Normalize();
    return *this;
  }

  Z& operator*=(const Z& other) {
    value_ *= other.value_;
    Normalize();
    return *this;
  }

  bool operator==(const Z& other) const {
    return value_ == other.value_ && size == other.size;
  }

  friend std::ostream& operator<<(std::ostream& os, const Z& number) {
    os << number.value_;
    return os;
  }

  friend std::istream& operator>>(std::istream& is, Z& number) {
    is >> number.value_;
    number.Normalize();
    return is;
  }

  friend struct std::hash<Z>;

  operator long long() const { return value_; }
};

namespace std {
template <>
struct hash<Z> {
  std::size_t operator()(const Z& obj) const {
    return std::hash<size_t>()(obj.value_);
  }
};
}  // namespace std

Z operator+(Z first, const Z& second) {
  first += second;
  return first;
}

Z operator*(Z first, const Z& second) {
  first *= second;
  return first;
}

Z Deg(Z base, size_t deg) {
  if (deg == 0) {
    return Z(base.size, 1);
  }

  Z recursive = Deg(base, deg / 2);

  return (deg % 2 == 0) ? recursive * recursive : recursive * recursive * base;
}

Z Reverse(const Z& number) {  //  irreversible => UB
  return Deg(number, number.size - 2);
}

Z& operator/=(Z& divisible, const Z& divisor) {
  divisible *= Reverse(divisor);
  return divisible;
}

Z operator/(Z first, const Z& second) {
  first /= second;
  return first;
}

bool IsResidue(Z number) { return Deg(number, (number.size - 1) / 2) == 1; }

Z FindNonresidue(size_t module) {
  for (size_t i = 1; i < module; ++i) {
    if (!IsResidue(Z(module, i))) {
      return Z(module, i);
    }
  }
  return Z(1, 0);
}

size_t Log2Ord(Z number) {
  size_t log_deg = 0;
  while (number != 1) {
    number *= number;
    ++log_deg;
  }

  return log_deg;
}

Z DiscreteSqrt(Z square) {
  if (square.size == 2) {
    return square;
  }

  if (square.size % 4 == 3) {
    return Deg(square, (square.size + 1) / 4);
  }

  size_t odd = square.size - 1;
  size_t deg2 = 0;
  while (odd % 2 == 0) {
    odd /= 2;  //  m
    ++deg2;
  }

  Z will_be_1 = Deg(square, odd);       //  u
  Z sqrt = Deg(square, (odd + 1) / 2);  //  v

  Z ord_deg2 = Deg(FindNonresidue(square.size), odd);  //  c

  while (will_be_1 != 1) {
    Z factor = ord_deg2;

    for (size_t i = 0; i < (deg2 - Log2Ord(will_be_1) - 1); ++i) {
      factor *= factor;
    }

    sqrt *= factor;
    will_be_1 *= factor * factor;
  }

  return sqrt;
}

int main() {
  size_t tests_num;
  std::cin >> tests_num;
  for (size_t k = 0; k < tests_num; ++k) {
    size_t number;
    long long prime;
    std::cin >> number >> prime;

    Z square(prime, number);

    if (IsResidue(square)) {
      std::cout << DiscreteSqrt(square) << '\n';
      continue;
    }

    std::cout << "IMPOSSIBLE\n";
  }
}
