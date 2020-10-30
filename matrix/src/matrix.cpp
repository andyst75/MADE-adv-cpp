#include "matrix.h"

#include <cmath>  // fabs && swap

using namespace task;

/*
 * Private methods
 */
void Matrix::check_bound(size_t row, size_t col) const {
  if (row >= rows || col >= cols) {
    throw OutOfBoundsException();
  }
}

void Matrix::check_size(const Matrix& a) const {
  if (a.getRows() != rows || a.getColumns() != cols) {
    throw SizeMismatchException();
  }
}

void Matrix::copy_array(const Matrix& from_array) {
  rows = from_array.getRows();
  cols = from_array.getColumns();
  array = new double[rows * cols];
  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      array[r * cols + c] = from_array.getRawArray()[r * cols + c];
    }
  }
}

/*
 * Constructors
 */
Matrix::Matrix() {
  array = new double[rows * cols];
  array[0] = 1;
}

Matrix::Matrix(size_t rows, size_t cols) {
  this->rows = rows;
  this->cols = cols;
  array = new double[rows * cols];
  for (size_t r = 0; r < rows; ++r) {
    for (size_t c = 0; c < cols; ++c) {
      array[r * cols + c] = (rows == cols && r == c) ? 1 : 0;
    }
  }
}

Matrix::Matrix(const Matrix& copy) { copy_array(copy); }

Matrix::Row::Row(Matrix& matrix, size_t row) : matrix(matrix), row(row) {}

Matrix::Row::Row(const Matrix& matrix, size_t row) : matrix(matrix), row(row) {}

/*
 * Public methods
 */
double& Matrix::get(size_t row, size_t col) {
  check_bound(row, col);
  return array[row * cols + col];
}

const double& Matrix::get(size_t row, size_t col) const {
  check_bound(row, col);
  return array[row * cols + col];
}

void Matrix::set(size_t row, size_t col, const double& value) {
  check_bound(row, col);
  array[row * cols + col] = value;
}

void Matrix::resize(size_t new_rows, size_t new_cols) {
  double* new_array = new double[new_rows * new_cols];

  for (size_t r = 0; r < new_rows; ++r) {
    for (size_t c = 0; c < new_cols; ++c) {
      new_array[r * new_cols + c] =
          (r < rows && c < cols) ? array[r * cols + c] : 0;
    }
  }

  rows = new_rows;
  cols = new_cols;

  delete[] array;
  array = new_array;
}

size_t Matrix::getRows() const { return rows; }

size_t Matrix::getColumns() const { return cols; }

task::Matrix& Matrix::operator=(const Matrix& a) {
  if (this == &a) {
    return *this;
  }

  delete[] array;
  copy_array(a);
  return *this;
}

task::Matrix& Matrix::operator+=(const Matrix& a) {
  check_size(a);
  for (size_t i = 0; i < rows * cols; ++i) {
    array[i] += a.getRawArray()[i];
  }

  return *this;
}

task::Matrix& Matrix::operator-=(const Matrix& a) {
  check_size(a);
  for (size_t i = 0; i < rows * cols; ++i) {
    array[i] -= a.getRawArray()[i];
  }

  return *this;
}

task::Matrix& Matrix::operator*=(const Matrix& a) {
  if (cols != a.getRows()) {
    throw SizeMismatchException();
  }

  Matrix old_matrix(*this);

  this->resize(rows, a.getColumns());

  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < a.getColumns(); ++j) {
      array[i * a.getColumns() + j] = 0;
      for (size_t k = 0; k < old_matrix.getColumns(); ++k)
        array[i * a.getColumns() + j] += old_matrix[i][k] * a[k][j];
    }
  }

  return *this;
}

task::Matrix& Matrix::operator*=(const double& number) {
  for (size_t i = 0; i < rows * cols; ++i) array[i] *= number;

  return *this;
}

task::Matrix Matrix::operator+(const Matrix& a) const {
  Matrix m(*this);
  m += a;
  return m;
}

task::Matrix Matrix::operator-(const Matrix& a) const {
  Matrix m(*this);
  m -= a;
  return m;
}

task::Matrix Matrix::operator*(const Matrix& a) const {
  Matrix m(*this);
  m *= a;
  return m;
}

task::Matrix Matrix::operator*(const double& a) const {
  Matrix m(*this);
  m *= a;
  return m;
}

task::Matrix task::operator*(const double& a, const Matrix& b) { return b * a; }

bool Matrix::operator==(const Matrix& a) const {
  for (size_t i = 0; i < rows * cols; ++i) {
    double tmp = array[i] - a.getRawArray()[i];
    if (std::fabs(tmp) > task::EPS) return false;
  }
  return true;
}

bool Matrix::operator!=(const Matrix& a) const { return !(*this == a); }

task::Matrix Matrix::operator-() const {
  Matrix m(*this);
  m *= -1.;
  return m;
}

task::Matrix Matrix::operator+() const { return Matrix(*this); }

task::Matrix::Row Matrix::operator[](size_t row) {
  if (row > rows) {
    throw OutOfBoundsException();
  }
  return Row(*this, row);
}

task::Matrix::Row Matrix::operator[](size_t row) const {
  if (row > rows) {
    throw OutOfBoundsException();
  }
  return Row(*this, row);
}

double& task::Matrix::Row::operator[](size_t col) {
  if (col > matrix.cols) {
    throw OutOfBoundsException();
  }
  return matrix.array[row * matrix.cols + col];
}

std::vector<double> Matrix::getRow(size_t row) {
  std::vector<double> v(cols);
  for (size_t i = 0; i < cols; ++i) v[i] = array[row * cols + i];
  return v;
}
std::vector<double> Matrix::getColumn(size_t column) {
  std::vector<double> v(rows);
  for (size_t i = 0; i < rows; ++i) v[i] = array[i * cols + column];
  return v;
}

// get access for raw data
double* Matrix::getRawArray() const { return array; }

std::ostream& task::operator<<(std::ostream& output,
                               const task::Matrix& matrix) {
  output.precision(12);
  for (size_t i = 0; i < matrix.getRows(); ++i)
    for (size_t j = 0; j < matrix.getColumns(); ++j)
      output << " " << matrix[i][j];
  output << std::endl;
  return output;
}

std::istream& task::operator>>(std::istream& input, task::Matrix& matrix) {
  size_t r = 0, c = 0;
  input >> r >> c;

  matrix.resize(r, c);

  for (size_t i = 0; i < r; ++i)
    for (size_t j = 0; j < c; ++j) input >> matrix[i][j];
  return input;
}

void Matrix::transpose() {
  Matrix old_matrix(*this);
  size_t old_rows = rows, old_cols = cols;
  resize(cols, rows);
  for (size_t i = 0; i < old_rows; ++i)
    for (size_t j = 0; j < old_cols; ++j)
      array[j * cols + i] = old_matrix.getRawArray()[i * old_cols + j];
}

task::Matrix Matrix::transposed() const {
  Matrix m(*this);
  m.transpose();
  return m;
}

double Matrix::trace() const {
  if (rows != cols) {
    throw SizeMismatchException();
  }
  double s = 0.;
  for (size_t i = 0; i < rows; ++i) {
    s += array[i * cols + i];
  }
  return s;
}

double Matrix::det() const {
  if (rows != cols) {
    throw SizeMismatchException();
  }

  double det = 1;

  for (size_t i = 0; i < rows - 1; ++i) {
    size_t max_col = i;
    double max_value = std::fabs(array[i * cols + i]);
    for (size_t k = i + 1; k < rows; ++k) {
      double val = std::fabs(array[k * cols + i]);
      if (val > max_value) {
        max_col = k;
        max_value = val;
      }
    }
    if (max_col != i) {
      for (size_t j = i; j < rows; ++j) {
        std::swap(array[i * cols + j], array[max_col * cols + j]);
      }
      det = -det;
    }

    double value = array[i * cols + i];
    if (std::fabs(value) < EPS) return 0.0;

    for (size_t r = i + 1; r < rows; ++r) {
      double k = array[r * cols + i] / value;
      for (size_t j = i; j < cols; j++) {
        array[r * cols + j] -= k * array[i * cols + j];
      }
    }
    det *= value;
  }

  det *= array[(rows - 1) * cols + cols - 1];
  return det;
}
