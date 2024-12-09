#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

//  n >= 2
template <size_t n>
class Z {
  long long value_;

  void Normalize() {
    if (value_ < 0) {
      value_ = -value_ % n;
      value_ *= -1;
      value_ += n;
    }
    value_ %= n;
  }

 public:
  explicit Z(long long value = 0) : value_(value) { Normalize(); }

  Z(const Z& other) : value_(other.value_) {}

  Z& operator=(const Z& other) {
    value_ = other.value_;

    return *this;
  }

  Z& operator+=(const Z& other) {
    value_ += other.value_;
    Normalize();
    return *this;
  }

  Z& operator-=(const Z& other) {
    value_ -= other.value_;
    Normalize();
    return *this;
  }

  Z& operator*=(const Z& other) {
    value_ *= other.value_;
    Normalize();
    return *this;
  }

  bool operator==(const Z& other) const { return value_ == other.value_; }

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

template <size_t n>
Z<n> operator+(Z<n> first, const Z<n>& second) {
  first += second;
  return first;
}

template <size_t n>
Z<n> operator-(Z<n> first, const Z<n>& second) {
  first -= second;
  return first;
}

template <size_t n>
Z<n> operator*(Z<n> first, const Z<n>& second) {
  first *= second;
  return first;
}

template <size_t n>
Z<n> Deg(Z<n> base, size_t deg) {
  if (deg == 0) {
    return Z<n>(1);
  }

  Z<n> recursive = Deg(base, deg / 2);

  return (deg % 2 == 0) ? recursive * recursive : recursive * recursive * base;
}

template <size_t n>
Z<n> Reverse(const Z<n>& number) {  //  irreversible => UB
  return Deg(number, n - 2);
}

template <size_t n>
Z<n>& operator/=(Z<n>& divisible, const Z<n>& divisor) {
  divisible *= Reverse(divisor);
  return divisible;
}

template <size_t n>
Z<n> operator/(Z<n> first, const Z<n>& second) {
  first /= second;
  return first;
}

const long long cPrime = 998244353;

using Field = Z<cPrime>;
using VecF = std::vector<Field>;

VecF InputPolynom() {
  size_t len;
  std::cin >> len;
  VecF polynom(len + 1);
  for (size_t i = 0; i < len + 1; ++i) {
    std::cin >> polynom[i];
  }

  return polynom;
}

Field FindRoot(size_t log2_deg) {
  const size_t cBase = 31;  //  ord = 2^23
  const size_t cOrdBase = 23;

  Field root = Field(cBase);
  for (size_t i = 0; i < cOrdBase - log2_deg; ++i) {
    root *= root;
  }

  return root;
}

size_t ReverseBitset(size_t bitset, size_t len) {
  size_t reversed = 0;

  for (size_t i = 0; i < len; ++i) {
    if ((bitset & (1 << i)) != 0U) {
      reversed |= 1 << (len - 1 - i);
    }
  }

  return reversed;
}

VecF FFT(VecF polynom, size_t log2_size, Field root) {
  polynom.resize(1 << log2_size, Field(0));

  VecF fft(polynom.size());
  for (size_t i = 0; i < fft.size(); ++i) {
    fft[i] = polynom[ReverseBitset(i, log2_size)];
  }

  VecF roots(1 << log2_size);
  roots[0] = Field(1);
  for (size_t i = 1; i < roots.size(); ++i) {
    roots[i] = roots[i - 1] * root;
  }

  for (size_t part_len = 2; part_len <= fft.size(); part_len *= 2) {
    size_t parts_num = fft.size() / part_len;
    for (size_t part_ind = 0; part_ind < parts_num; ++part_ind) {
      for (size_t i = 0; i < part_len / 2; ++i) {  //  index inside part
        size_t index = part_len * part_ind + i;
        Field from_even = fft[index];
        Field from_odd = fft[index + part_len / 2];

        fft[index] = from_even + roots[i * parts_num] * from_odd;
        fft[index + part_len / 2] = from_even - roots[i * parts_num] * from_odd;
      }
    }
  }

  return fft;
}

template <typename T>
std::vector<T> FindDotProduct(const std::vector<T>& first,
                              const std::vector<T>& second) {
  size_t len = std::min(first.size(), second.size());

  std::vector<T> dot_product(len);
  for (size_t i = 0; i < len; ++i) {
    dot_product[len - 1 - i] =
        first[first.size() - 1 - i] * second[second.size() - 1 - i];
  }

  return dot_product;
}

VecF FindProduct(VecF first, VecF second) {
  size_t product_len = first.size() + second.size();

  size_t log2_points_num = 0;
  size_t tmp = product_len + 1;
  while (tmp > 0) {
    ++log2_points_num;
    tmp /= 2;
  }

  Field root = FindRoot(log2_points_num);

  std::reverse(first.begin(), first.end());
  std::reverse(second.begin(), second.end());

  VecF values_first = FFT(first, log2_points_num, root);
  VecF values_second = FFT(second, log2_points_num, root);

  VecF dot_product = FindDotProduct(values_first, values_second);

  VecF product = FFT(dot_product, log2_points_num, Reverse(root));
  Field divisor(1 << log2_points_num);

  for (size_t i = 0; i < product.size(); ++i) {
    product[i] /= divisor;
  }

  while (!product.empty() && product.back() == Field(0)) {
    product.pop_back();
  }

  std::reverse(product.begin(), product.end());

  return product;
}

int main() {
  VecF first = InputPolynom();
  VecF second = InputPolynom();

  VecF product = FindProduct(first, second);

  std::cout << product.size() - 1 << ' ';

  const long long cTooBig = 1e7;

  for (size_t i = 0; i < product.size(); ++i) {
    long long number = static_cast<long long>(product[i]);
    std::cout << ((number > cTooBig) ? number - cPrime : number) << ' ';
  }
}
