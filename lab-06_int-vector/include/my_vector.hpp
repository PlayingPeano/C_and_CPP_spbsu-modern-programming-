#pragma once

#include <cstddef>

class MyVector {
public:
    MyVector();
    MyVector(std::size_t init_capacity);
    MyVector(const MyVector& that);
    ~MyVector();

    void set(std::size_t index, int value);
    int get(std::size_t index);

    std::size_t size() const;
    std::size_t capacity() const;

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size);

    void push_back(int value);
    void insert(std::size_t index, int value);
    void erase(std::size_t index);

    friend void swap(MyVector& first, MyVector& second);
    MyVector& operator=(const MyVector& other);
private:
    std::size_t _size, _capacity{};
    int * _data;
};