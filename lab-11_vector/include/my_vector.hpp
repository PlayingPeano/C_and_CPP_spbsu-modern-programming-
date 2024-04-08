#ifndef MY_VECTOR_H_
#define MY_VECTOR_H_

#include <cstddef>
#include <iostream>
#include <type_traits>

namespace containers
{
    template<typename T>
    class my_vector
    {
    public:
        my_vector();

        explicit my_vector(std::size_t n);

        my_vector(const my_vector &other);

        my_vector &operator=(my_vector other); //сделать swap для него
        ~my_vector();

        std::size_t size() const;

        std::size_t capacity() const;

        bool empty() const;

        void resize(std::size_t n);

        void reserve(std::size_t n);

        T &operator[](std::size_t index) const;

        void push_back(T t);

        void pop_back();

        void clear();

        template<typename U>
        friend std::ostream &operator<<(std::ostream &os, const my_vector<U> &v);

    private:
        size_t capacity_;
        size_t size_;
        T *array_;

        void swap(my_vector &other);

        my_vector return_copy_and_clear();
    };
}// namespace containers

#include "my_vector_impl.hpp"

#endif  // MY_VECTOR_H_
