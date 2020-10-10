#pragma once

#include <iostream>
#include <vector>

namespace task {

const double EPS = 1e-6;

class OutOfBoundsException : public std::exception {};
class SizeMismatchException : public std::exception {};

class Matrix {
 public:
  /*
   * Proxy-class for matrix row
   */
  class Row {
    friend class Matrix;

   private:
    const Matrix& matrix;
    const size_t row;

    Row(Matrix& matrix, size_t row);
    Row(const Matrix& matrix, size_t row);

   public:
    double& operator[](size_t col);
  };

  Matrix();
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& copy);
  Matrix& operator=(const Matrix& a);

  double& get(size_t row, size_t col);
  const double& get(size_t row, size_t col) const;
  void set(size_t row, size_t col, const double& value);
  void resize(size_t new_rows, size_t new_cols);

  Row operator[](size_t row);
  Row operator[](size_t row) const;

  Matrix& operator+=(const Matrix& a);
  Matrix& operator-=(const Matrix& a);
  Matrix& operator*=(const Matrix& a);
  Matrix& operator*=(const double& number);

  Matrix operator+(const Matrix& a) const;
  Matrix operator-(const Matrix& a) const;
  Matrix operator*(const Matrix& a) const;
  Matrix operator*(const double& a) const;

  Matrix operator-() const;
  Matrix operator+() const;

  double det() const;
  void transpose();
  Matrix transposed() const;
  double trace() const;

  std::vector<double> getRow(size_t row);
  std::vector<double> getColumn(size_t column);

  bool operator==(const Matrix& a) const;
  bool operator!=(const Matrix& a) const;

  size_t getRows() const;
  size_t getColumns() const;
  Matrix(size_t cols, double* column);
  double* getRawArray() const;

  ~Matrix() { delete[] array; }

 private:
  size_t rows = 1;
  size_t cols = 1;
  double* array = nullptr;

  void check_bound(size_t row, size_t col) const;
  void check_size(const Matrix& a) const;
  void copy_array(const Matrix& from_array);
};

Matrix operator*(const double& a, const Matrix& b);

std::ostream& operator<<(std::ostream& output, const Matrix& matrix);
std::istream& operator>>(std::istream& input, Matrix& matrix);

}  // namespace task
