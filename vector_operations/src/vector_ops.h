#pragma once
#include <cmath>
#include <iostream>
#include <vector>

namespace task {

// binary +
std::vector<double> operator+(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> result(a.size());
  for (size_t i = 0; i < result.size(); i++) {
    result[i] = a[i] + b[i];
  }
  return result;
}

// binary -
std::vector<double> operator-(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> result(a.size());
  for (size_t i = 0; i < result.size(); i++) {
    result[i] = a[i] - b[i];
  }
  return result;
}

// unary +
std::vector<double> operator+(const std::vector<double>& vector) {
  return vector;
}

// unary -
std::vector<double> operator-(const std::vector<double>& vector) {
  std::vector<double> result(vector.size());
  for (size_t i = 0; i < result.size(); i++) {
    result[i] = -vector[i];
  }
  return result;
}

// scalar multiply
double operator*(const std::vector<double>& a, const std::vector<double>& b) {
  double result = 0;
  for (size_t i = 0; i < a.size(); i++) {
    result += a[i] * b[i];
  }
  return result;
}

// vector multiply, only 3-dim
std::vector<double> operator%(const std::vector<double>& a,
                              const std::vector<double>& b) {
  std::vector<double> result(a);
  if (result.size() == 3) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
  }
  return result;
}

// collinearity operator
bool operator||(const std::vector<double>& a, const std::vector<double>& b) {
  const double EPS = 1e-12;
  size_t i = 0;
  while (i < b.size() && !a[i] && !b[i]) {
    i++;
  }
  if (i == b.size() || !a[i] || !b[i]) {
    return false;
  } else {
    double k = a[i] / b[i];
    for (size_t j = i + 1; j < b.size(); j++) {
      if (fabs(k * b[j] - a[j]) > EPS) {
        return false;
      }
    }
    return true;
  }
}

// codirectionality operator
bool operator&&(const std::vector<double>& a, const std::vector<double>& b) {
  return (a || b) && (a * b > 0);
}

// stream vector input
std::istream& operator>>(std::istream& stream, std::vector<double>& vector) {
  long n = 0;
  stream >> n;
  vector.resize(n);
  for (auto& v : vector) {
    stream >> v;
  }
  return stream;
}

// stream vector output
std::ostream& operator<<(std::ostream& stream,
                         const std::vector<double>& vector) {
  if (vector.size() > 0) {
    stream << vector[0];
    for (size_t i = 1; i < vector.size(); i++) {
      stream << " " << vector[i];
    }
    stream << '\n';
  }
  return stream;
}

// reverse vector
void reverse(std::vector<double>& vector) {
  size_t size = vector.size();
  for (size_t i = 0; i < size / 2; i++) {
    std::swap(vector[i], vector[size - i - 1]);
  }
}

// bitwise OR
std::vector<int> operator|(const std::vector<int>& a,
                           const std::vector<int>& b) {
  std::vector<int> result(std::min(a.size(), b.size()));
  for (size_t i = 0; i < result.size(); i++) result[i] = a[i] | b[i];
  return result;
}

// bitwise AND
std::vector<int> operator&(const std::vector<int>& a,
                           const std::vector<int>& b) {
  std::vector<int> result(std::min(a.size(), b.size()));
  for (size_t i = 0; i < result.size(); i++) result[i] = a[i] & b[i];
  return result;
}

}  // namespace task
