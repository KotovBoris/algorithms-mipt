#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

const long long cPrime = 7340033;
const long long cMod = 7;
const size_t cBase = 5950657;  //  ord = 2^20
const size_t cOrdBase = 20;

//  n >= 2
class Z {
 public:
  static size_t n;

  void Normalize() {
    if (value_ < 0) {
      value_ = -value_ % n;
      value_ *= -1;
      value_ += n;
    }
    value_ %= n;
  }

 private:
  long long value_;

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

size_t Z::n = cMod;

Z operator+(Z first, const Z& second) {
  first += second;
  return first;
}

Z operator-(Z first, const Z& second) {
  first -= second;
  return first;
}

Z operator*(Z first, const Z& second) {
  first *= second;
  return first;
}

Z Deg(Z base, size_t deg) {
  if (deg == 0) {
    return Z(1);
  }

  Z recursive = Deg(base, deg / 2);

  return (deg % 2 == 0) ? recursive * recursive : recursive * recursive * base;
}

std::unordered_map<long long, long long> cash;

Z Reverse(const Z& number) {  //  irreversible => UB
  long long ll = static_cast<long long>(number);
  if (cash.contains(ll)) {
    return Z(cash[ll]);
  }

  Z ans = Deg(number, Z::n - 2);
  cash[static_cast<long long>(number)] = static_cast<long long>(ans);
  return ans;
}

Z& operator/=(Z& divisible, const Z& divisor) {
  divisible *= Reverse(divisor);
  return divisible;
}

Z operator/(Z first, const Z& second) {
  first /= second;
  return first;
}

using VecZ = std::vector<Z>;

VecZ InputPolynom() {
  size_t len;
  std::cin >> len;
  VecZ polynom(len + 1);
  for (size_t i = 0; i < len + 1; ++i) {
    std::cin >> polynom[i];
  }

  std::reverse(polynom.begin(), polynom.end());

  return polynom;
}

Z FindRoot(size_t log2_deg) {
  Z root = Z(cBase);
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

VecZ FFT(VecZ polynom, size_t log2_size, Z root) {
  polynom.resize(1 << log2_size, Z(0));

  VecZ fft(polynom.size());
  for (size_t i = 0; i < fft.size(); ++i) {
    fft[i] = polynom[ReverseBitset(i, log2_size)];
  }

  VecZ roots(1 << log2_size);
  roots[0] = Z(1);
  for (size_t i = 1; i < roots.size(); ++i) {
    roots[i] = roots[i - 1] * root;
  }

  for (size_t part_len = 2; part_len <= fft.size(); part_len *= 2) {
    size_t parts_num = fft.size() / part_len;
    for (size_t part_ind = 0; part_ind < parts_num; ++part_ind) {
      for (size_t i = 0; i < part_len / 2; ++i) {  //  index inside part
        size_t index = part_len * part_ind + i;
        Z from_even = fft[index];
        Z from_odd = fft[index + part_len / 2];

        fft[index] = from_even + roots[i * parts_num] * from_odd;
        fft[index + part_len / 2] = from_even - roots[i * parts_num] * from_odd;
      }
    }
  }

  return fft;
}

VecZ FindDotProduct(const VecZ& first, const VecZ& second) {
  size_t len = std::min(first.size(), second.size());

  VecZ dot_product(len);
  for (size_t i = 0; i < len; ++i) {
    dot_product[len - 1 - i] =
        first[first.size() - 1 - i] * second[second.size() - 1 - i];
  }

  return dot_product;
}

size_t CeilLog2(size_t number) {
  if (number == 0) {
    return 0;
  }

  size_t ceil_log2 = 0;
  while (number > 0) {
    ++ceil_log2;
    number /= 2;
  }

  return ceil_log2;
}

void DeleteLastZeros(VecZ& poly) {
  while (!poly.empty() && poly.back() == Z(0)) {
    poly.pop_back();
  }
}

VecZ operator*(VecZ first, VecZ second) {
  size_t old_n = Z::n;
  Z::n = cPrime;

  size_t product_len = first.size() + second.size();

  size_t log2_points_num = CeilLog2(product_len + 1);

  Z root = FindRoot(log2_points_num);

  VecZ values_first = FFT(first, log2_points_num, root);
  VecZ values_second = FFT(second, log2_points_num, root);

  VecZ dot_product = FindDotProduct(values_first, values_second);

  VecZ product = FFT(dot_product, log2_points_num, Reverse(root));
  Z divisor(1 << log2_points_num);

  for (size_t i = 0; i < product.size(); ++i) {
    product[i] /= divisor;
  }

  DeleteLastZeros(product);

  if (Z::n == old_n) {
    return product;
  }

  Z::n = old_n;
  for (size_t i = 0; i < product.size(); ++i) {
    product[i].Normalize();
  }

  return product;
}

VecZ operator+(const VecZ& first, const VecZ& second) {
  if (first.size() < second.size()) {
    return second + first;
  }

  VecZ sum = first;
  for (size_t i = 0; i < second.size(); ++i) {
    sum[i] += second[i];
  }

  return sum;
}

VecZ operator-(VecZ poly) {
  Z minus_one(-1);
  for (size_t i = 0; i < poly.size(); ++i) {
    poly[i] *= minus_one;
  }

  return poly;
}

VecZ operator-(const VecZ& first, const VecZ& second) {
  return first + -second;
}

VecZ Mod(VecZ poly, size_t x_deg) {
  poly.resize(x_deg);
  return poly;
}

VecZ Div(VecZ poly, size_t x_deg) {
  if (poly.size() <= x_deg) {
    return {};
  }

  poly.erase(poly.begin(), poly.begin() + x_deg);

  return poly;
}

VecZ Concatenate(VecZ first, const VecZ& second) {
  first.insert(first.end(), second.begin(), second.end());

  return first;
}

VecZ ReversePoly(size_t x_deg, const VecZ& poly) {
  if (x_deg == 1) {
    return {Reverse(poly[0])};
  }

  size_t half = x_deg / 2;

  VecZ prefix = ReversePoly(half, poly);

  VecZ poly_prefix = Mod(poly, half);
  VecZ poly_suffix = Mod(Div(poly, half), half);
  VecZ product_suffix = Mod(Div(prefix * poly_prefix, half), half);
  VecZ suffix = Mod(-prefix * (product_suffix + prefix * poly_suffix), half);

  return Concatenate(prefix, suffix);
}

VecZ Invert(VecZ poly) {
  std::reverse(poly.begin(), poly.end());
  DeleteLastZeros(poly);

  return poly;
}

std::pair<VecZ, VecZ> Divide(const VecZ& divisible, const VecZ& divisor) {
  if (divisible.size() < divisor.size()) {
    return std::make_pair(VecZ{}, divisible);
  }

  size_t deg = divisible.size() - divisor.size() + 1;
  size_t good_deg = 1 << CeilLog2(deg);

  VecZ reversed = ReversePoly(good_deg, Invert(divisor));

  VecZ quotient = Invert(divisible) * reversed;
  quotient = Invert(Mod(quotient, deg));

  return std::make_pair(quotient, divisible - divisor * quotient);
}

void Print(const VecZ& poly) {
  long long index = poly.size() - 1;

  while (index >= 0 && poly[index] == Z(0)) {
    --index;
  }

  if (index < 0) {
    std::cout << "0 0\n";
    return;
  }

  std::cout << index << ' ';
  for (long long i = 0; i <= index; ++i) {
    std::cout << poly[index - i] << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  VecZ divisible = InputPolynom();
  VecZ divisor = InputPolynom();

  std::pair<VecZ, VecZ> buffer = Divide(divisible, divisor);

  Print(buffer.first);
  Print(buffer.second);

  return 0;
}
