#include "matrix.h"
#include "stdexcept"
#include "fstream"
#include "algorithm"

void swap(Matrix &a, Matrix &b)
{
    std::swap(a._rows, b._rows);
    std::swap(a._cols, b._cols);
    std::swap(a._data, b._data);
}

Matrix::Matrix(size_t r, size_t c)
{
    _rows = r;
    _cols = c;
    _data = new int *[_rows];
    int *mem = new int[_rows * _cols];
    for (size_t i = 0; i < _rows; ++i)
    {
        _data[i] = &mem[i * _cols];
    }
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _cols; ++j)
        {
            _data[i][j] = 0;
        }
    }
}

Matrix::Matrix(const Matrix &m)
{
    Matrix temp(m._rows, m._cols);
    for (size_t i = 0; i < m._rows; ++i)
    {
        for (size_t j = 0; j < m._cols; ++j)
        {
            temp._data[i][j] = m._data[i][j];
        }
    }

    swap(*this, temp);
}

Matrix::~Matrix()
{
    if (_data != nullptr)
    {
        delete[] _data[0];
    }
    delete[] _data;
}

size_t Matrix::get_rows() const
{
    return _rows;
}

size_t Matrix::get_cols() const
{
    return _cols;
}

void Matrix::set(size_t i, size_t j, int val)
{
    if (i >= _rows || j >= _cols)
    {
        throw std::out_of_range("index out of range");
    }
    _data[i][j] = val;
}

int Matrix::get(size_t i, size_t j) const
{
    if (i >= _rows || j >= _cols)
    {
        throw std::out_of_range("index out of range");
    }
    return _data[i][j];
}

void Matrix::print(FILE *f) const
{
    if (f == nullptr)
    {
        throw std::invalid_argument("Failed to open file");
    }
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _cols; ++j)
        {
            if (j != _cols - 1)
            {
                fprintf(f, "%d ", _data[i][j]);
            } else
            {
                fprintf(f, "%d", _data[i][j]);
            }
        }
        fprintf(f, "\n");
    }
}

Matrix Matrix::operator+(const Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
    {
        throw std::invalid_argument("Matrices have different sizes");
    }

    Matrix temp(*this);
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _cols; ++j)
        {
            temp._data[i][j] += m._data[i][j];
        }
    }
    return temp;
}

Matrix Matrix::operator-(const Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
    {
        throw std::invalid_argument("Matrices have different sizes");
    }

    return *this + m * (-1);
}


Matrix Matrix::operator*(const Matrix &m) const
{
    if (_cols != m._rows)
    {
        throw std::invalid_argument("Matrices have different sizes");
    }

    Matrix temp(_rows, m._cols);
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < m._cols; ++j)
        {
            for (size_t k = 0; k < _cols; ++k)
            {
                temp._data[i][j] += _data[i][k] * m._data[k][j];
            }
        }
    }
    return temp;
}

Matrix Matrix::operator*(int x) const
{
    Matrix temp(*this);
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _cols; ++j)
        {
            temp._data[i][j] *= x;
        }
    }
    return temp;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
    if (_rows != m._rows || _cols != m._cols)
    {
        throw std::invalid_argument("Matrices have different sizes");
    }

    Matrix temp(*this + m);
    swap(*this, temp);
    return *this;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
    return *this += m * (-1);
}

Matrix &Matrix::operator*=(const Matrix &m)
{
    Matrix temp(*this * m);
    swap(*this, temp);
    return *this;
}

bool Matrix::operator==(const Matrix &m) const
{
    if (_rows != m._rows || _cols != m._cols)
    {
        return false;
    }
    for (size_t i = 0; i < _rows; ++i)
    {
        for (size_t j = 0; j < _cols; ++j)
        {
            if (_data[i][j] != m._data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &m) const
{
    return !(*this == m);
}

Matrix &Matrix::operator=(const Matrix &m)
{
    if (this == &m)
    {
        return *this;
    }
    Matrix temp(m);
    swap(*this, temp);
    return *this;
}
