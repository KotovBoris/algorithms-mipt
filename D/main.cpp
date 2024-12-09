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
  Z(size_t size, long long value = 0) : size(size), value_(value) {
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

template <typename T, typename U>
using Hashmap = std::unordered_map<T, U>;

std::string DiscreteLog(Z base, Z number) {
  if (base.size != number.size) {
    return "bad input";
  }

  size_t prime = base.size;

  size_t sqrt_prime = std::ceil(std::sqrt(prime));

  Hashmap<Z, size_t> degrees;

  Z current(prime, 1);
  degrees[current] = 0;
  for (size_t i = 1; i <= sqrt_prime; ++i) {
    current *= base;
    if (!degrees.contains(current)) {
      degrees[current] = i;
    }
  }

  Z step = Reverse(current);
  current = number;
  for (size_t i = 0; i <= sqrt_prime; ++i) {
    if (degrees.contains(current)) {
      return std::to_string(degrees[current] + i * sqrt_prime);
    }

    current *= step;
  }

  return "no solution";
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  size_t prime;
  while (std::cin >> prime) {
    Z base(prime);
    Z number(prime);

    std::cin >> base >> number;

    std::cout << DiscreteLog(base, number) << '\n';
  }
  fclose(stdin);
  fclose(stdout);
}
