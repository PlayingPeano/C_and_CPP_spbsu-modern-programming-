#include "my_vector.hpp"
#include <stdexcept>
#include <algorithm>
#include <cstring>

MyVector::MyVector()
{
    _size = 0;
    _capacity = 0;
    _data = nullptr;
}

MyVector::MyVector(std::size_t init_capacity)
{
    _size = init_capacity;
    _capacity = 0;
    _data = new int[init_capacity];
}

MyVector::MyVector(const MyVector &that)
{
    _data = new int[that._capacity];
    _size = that._size;
    _capacity = that._capacity;
    std::memcpy(_data, that._data, _size * sizeof(int));
}

void swap(MyVector& first, MyVector& second)
{
    std::swap(first._size, second._size);
    std::swap(first._capacity, second._capacity);
    std::swap(first._data, second._data);
}

MyVector &MyVector::operator=(const MyVector &other)
{
    MyVector tmp(other);
    std::swap(*this, tmp);
    return *this;
}

MyVector::~MyVector()
{
    delete[] _data;
}

void MyVector::set(std::size_t index, int value)
{
    if (index >= _size)
    {
        throw std::out_of_range("index out of range");
    }
    _data[index] = value;
}

int MyVector::get(std::size_t index)
{
    if (index >= _size)
    {
        throw std::out_of_range("index out of range");
    }
    return _data[index];
}

std::size_t MyVector::size() const
{
    return _size;
}

std::size_t MyVector::capacity() const
{
    return _capacity;
}

void MyVector::reserve(std::size_t new_capacity)
{
    if (new_capacity <= _capacity)
    {
        return;
    }
    int *new_data = new int[new_capacity];
    memcpy(new_data, _data, _size * sizeof(int));
    delete[] _data;

    _data = new_data;
    for (size_t i = _size; i < new_capacity; ++i)
    {
        _data[i] = 0;
    }
    _capacity = new_capacity;
}

void MyVector::resize(std::size_t new_size)
{
    if (new_size <= _capacity)
    {
        _size = new_size;
        return;
    }

    reserve(std::max(new_size, 2 * _size));
    _size = new_size;
}

void MyVector::push_back(int value)
{
    resize(size() + 1);
    _data[size() - 1] = value;
}

void MyVector::insert(std::size_t index, int value)
{
    if (index > _size)
    {
        throw std::out_of_range("index out of range");
    }

    resize(_size + 1);

    for (size_t i = _size - 1; i > index; --i)
    {
        _data[i] = _data[i - 1];
    }
    _data[index] = value;
}

void MyVector::erase(std::size_t index)
{
    if (_size == 0 || index >= _size)
    {
        throw std::out_of_range("there are no elements to delete");
    }
    for (size_t i = index; i < _size - 1; ++i)
    {
        _data[i] = _data[i + 1];
    }
    --_size;
}

