#pragma once

#include <cstdio>
#include <cstddef>

class Matrix {
public:
  friend void swap(Matrix &a, Matrix &b);
  Matrix(size_t r, size_t c);
  Matrix(const Matrix& m);

  size_t get_rows() const;
  size_t get_cols() const;
  void set(size_t i, size_t j, int val);
  int get(size_t i, size_t j) const;
  void print(FILE *f) const;

  Matrix operator+(const Matrix& m) const;
  Matrix operator-(const Matrix& m) const;
  Matrix operator*(const Matrix& m) const;
  Matrix operator*(const int x) const;

  Matrix& operator+=(const Matrix& m);
  Matrix& operator-=(const Matrix& m);
  Matrix& operator*=(const Matrix& m);

  bool operator==(const Matrix& m) const;
  bool operator!=(const Matrix& m) const;

private:
  size_t _rows{};
  size_t _cols{};
  int **_data{};
};