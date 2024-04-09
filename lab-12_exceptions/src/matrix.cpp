#include "matrix.h"

#include <utility>
#include "stdexcept"
#include "fstream"
#include "algorithm"
#include <iostream>
#include <limits>

namespace matrix
{
    MatrixException::MatrixException(std::string ex) : message_(std::move(ex))
    {}

    const char *MatrixException::what() const noexcept
    {
        return message_.c_str();
    }

    void Matrix::swap(Matrix &a, Matrix &b)
    {
        std::swap(a._rows, b._rows);
        std::swap(a._cols, b._cols);
        std::swap(a._data, b._data);
    }

    Matrix::Matrix()
    {
        _rows = 0;
        _cols = 0;
        _data = nullptr;
    }

    Matrix::Matrix(const std::string &filename)
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            throw MatrixException("LOAD: unable to open file.");
        }
        file >> _rows;
        if (file.fail() || file.bad())
        {
            throw MatrixException("LOAD: invalid file format.");
        }
        file >> _cols;
        if (file.fail() || file.bad())
        {
            throw MatrixException("LOAD: invalid file format.");
        }
        try
        {
            _data = new int *[_rows];
            int *mem = new int[_rows * _cols];
            for (std::size_t i = 0; i < _rows; ++i)
            {
                _data[i] = mem + i * _cols;
            }
        }
        catch (MatrixException &e)
        {
            throw e;
        }
        catch (std::bad_alloc &e)
        {
            throw e;
        }
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < _cols; ++j)
            {
                int x = 0;
                file >> x;
                if (file.fail() || file.bad())
                {
                    throw MatrixException("LOAD: invalid file format.");
                }
                _data[i][j] = x;
            }
        }
    }

    Matrix::Matrix(std::size_t r, std::size_t c)
    {
        _rows = r;
        _cols = c;
        int *mem;
        try
        {
            _data = new int *[_rows];
            mem = new int[_rows * _cols];
        }
        catch (std::bad_alloc &e)
        {
            delete[]mem;
            throw e;
        }

        for (std::size_t i = 0; i < _rows; ++i)
        {
            _data[i] = &mem[i * _cols];
        }
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < _cols; ++j)
            {
                _data[i][j] = 0;
            }
        }
    }

    Matrix::Matrix(const Matrix &m) : Matrix(m._rows, m._cols)
    {
        for (std::size_t i = 0; i < m._rows; ++i)
        {
            for (std::size_t j = 0; j < m._cols; ++j)
            {
                _data[i][j] = m._data[i][j];
            }
        }
    }

    Matrix::~Matrix()
    {
        if (_data != nullptr)
        {
            delete[] _data[0];
        }
        delete[] _data;
    }

    std::size_t Matrix::get_rows() const
    {
        return _rows;
    }

    std::size_t Matrix::get_cols() const
    {
        return _cols;
    }

    void Matrix::set(std::size_t i, std::size_t j, int val)
    {
        if (i >= _rows || j >= _cols)
        {
            throw std::out_of_range("index out of range");
        }
        _data[i][j] = val;
    }

    int Matrix::get(std::size_t i, std::size_t j) const
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
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < _cols; ++j)
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

    Matrix Matrix::signedSum(const Matrix &host, const Matrix &m, bool sign)
    {
        if (host._rows != m._rows || host._cols != m._cols)
        {
            throw MatrixException("dimensions do not match.");
        }

        Matrix temp(host);
        for (std::size_t i = 0; i < host._rows; ++i)
        {
            for (std::size_t j = 0; j < host._cols; ++j)
            {
                temp._data[i][j] += m._data[i][j] * (sign ? -1 : 1);
            }
        }
        return temp;
    }

    Matrix Matrix::operator+(const Matrix &m) const
    {
        return signedSum(*this, m, false);
    }

    Matrix Matrix::operator-(const Matrix &m) const
    {
        return signedSum(*this, m, true);
    }

    Matrix Matrix::operator*(const Matrix &m) const
    {
        if (_cols != m._rows)
        {
            throw MatrixException("#arg1.columns != #arg2.rows.");
        }

        Matrix temp(_rows, m._cols);
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < m._cols; ++j)
            {
                for (std::size_t k = 0; k < _cols; ++k)
                {
                    long long temp_val = temp._data[i][j] + _data[i][k] * m._data[k][j];
                    if (temp_val > std::numeric_limits<int>::max() || temp_val < std::numeric_limits<int>::min())
                    {
                        throw std::bad_alloc();
                    }
                    temp._data[i][j] += _data[i][k] * m._data[k][j];
                }
            }
        }
        return temp;
    }

    Matrix Matrix::operator*(int x) const
    {
        Matrix temp(*this);
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < _cols; ++j)
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
            throw MatrixException("dimensions do not match.");
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
        if (_cols != m._rows)
        {
            throw MatrixException("#arg1.columns != #arg2.rows.");
        }

        try
        {
            Matrix temp(*this * m);
            swap(*this, temp);
        }
        catch (std::bad_alloc &e)
        {
            throw e;
        }
        return *this;
    }

    bool Matrix::operator==(const Matrix &m) const
    {
        if (_rows != m._rows || _cols != m._cols)
        {
            return false;
        }
        for (std::size_t i = 0; i < _rows; ++i)
        {
            for (std::size_t j = 0; j < _cols; ++j)
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

    int Matrix::operator()(std::size_t i, std::size_t j)
    {
        if (i >= _rows || j >= _cols)
        {
            throw MatrixException("bad index.");
        }
        return _data[i][j];
    }

    std::istream &operator>>(std::istream &is, Matrix &m)
    {
        for (std::size_t i = 0; i < m._rows; ++i)
        {
            for (std::size_t j = 0; j < m._cols; ++j)
            {
                if (is.eof())
                {
                    throw MatrixException("invalid file format.");
                }
                is >> m._data[i][j];
            }
        }
        return is;
    }

    std::ostream &operator<<(std::ostream &os, const Matrix &m)
    {
        for (std::size_t i = 0; i < m._rows; ++i)
        {
            for (std::size_t j = 0; j < m._cols; ++j)
            {
                os << m._data[i][j] << ((j == m._cols - 1) ? "" : " ");
            }
            os << std::endl;
        }
        return os;
    }
}