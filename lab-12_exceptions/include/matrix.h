#pragma once

#include <cstdio>
#include <cstddef>
#include <string>
#include <exception>

namespace matrix
{
    class MatrixException : public std::exception
    {
    private:
        std::string message_;
    public:
        MatrixException(std::string ex);

        const char *what() const noexcept override;
    };

    class Matrix
    {
    public:
        Matrix();

        Matrix(const std::string &filename);

        Matrix(std::size_t r, std::size_t c);

        Matrix(const Matrix &m);

        ~Matrix();

        std::size_t get_rows() const;

        std::size_t get_cols() const;

        void set(std::size_t i, std::size_t j, int val);

        int get(std::size_t i, std::size_t j) const;

        void print(FILE *f) const;

        Matrix operator+(const Matrix &m) const;

        Matrix operator-(const Matrix &m) const;

        Matrix operator*(const Matrix &m) const;

        Matrix operator*(int x) const;

        Matrix &operator=(const Matrix &m);

        Matrix &operator+=(const Matrix &m);

        Matrix &operator-=(const Matrix &m);

        Matrix &operator*=(const Matrix &m);

        bool operator==(const Matrix &m) const;

        bool operator!=(const Matrix &m) const;

        int operator()(std::size_t i, std::size_t j);

        friend std::istream &operator>>(std::istream &is, Matrix &m);

        friend std::ostream &operator<<(std::ostream &os, const Matrix &m);


    private:
        std::size_t _rows{};
        std::size_t _cols{};
        int **_data{};

        static void swap(Matrix &a, Matrix &b);

        static Matrix signedSum(const Matrix &host, const Matrix &m, bool sign);
    };
}