#pragma once

#include <cstddef>
#include <stdexcept>
#include <bitset>

namespace constants
{
    const std::byte BYTE_EMPTY_MASK{0x00};
    const std::byte BYTE_FULL_MASK{0xFF};
} // namespace constants

namespace help_functions
{
    void set_bit(std::byte &byte, std::size_t index, bool val)
    {
        if (val)
        {
            byte |= static_cast<std::byte>(1 << index);
        } else
        {
            byte &= constants::BYTE_FULL_MASK ^ static_cast<std::byte>(1 << index);
        }
    }

    bool get_bit(std::byte byte, std::size_t pos)
    {
        return static_cast<bool>(byte & static_cast<std::byte>(1 << pos));
    }
} // namespace help_functions

namespace lab_13
{
    template<typename T, std::size_t N>
    class my_array
    {
    public:
        T &at(std::size_t index);

        const T &at(std::size_t index) const;

        T &operator[](std::size_t index);

        const T &operator[](std::size_t index) const;

        bool empty() const;

        std::size_t size() const;

        void fill(T val);

    private:
        T _data[N];
    };


    template<typename T, std::size_t N>
    T &my_array<T, N>::at(std::size_t index)
    {
        if (index >= N)
        {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::at(std::size_t index) const
    {
        if (index >= N)
        {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    template<typename T, std::size_t N>
    T &my_array<T, N>::operator[](std::size_t index)
    {
        return _data[index];
    }

    template<typename T, std::size_t N>
    const T &my_array<T, N>::operator[](std::size_t index) const
    {
        return _data[index];
    }

    template<typename T, std::size_t N>
    bool my_array<T, N>::empty() const
    {
        return N == 0;
    }

    template<typename T, std::size_t N>
    std::size_t my_array<T, N>::size() const
    {
        return N;
    }

    template<typename T, std::size_t N>
    void my_array<T, N>::fill(T val)
    {
        for (std::size_t i = 0; i < N; i++)
        {
            _data[i] = val;
        }
    }

    class proxy_byte
    {
    public:
        proxy_byte() = delete;

        proxy_byte(std::byte &byte, std::size_t bit_position) : _byte(byte), _bit_index(bit_position)
        {}

        proxy_byte &operator=(bool val);

        proxy_byte &operator=(const proxy_byte &pb);

        operator bool() const;

    private:
        std::byte &_byte;
        std::size_t _bit_index;
    };

    proxy_byte &proxy_byte::operator=(bool val)
    {
        help_functions::set_bit(_byte,_bit_index, val);
        return *this;
    }

    proxy_byte &proxy_byte::operator=(const proxy_byte &pb)
    {
        if (this != &pb)
        {
            help_functions::set_bit(_byte, _bit_index, pb.operator bool());
        }
        return *this;
    }

    proxy_byte::operator bool() const
    {
        return help_functions::get_bit(_byte, _bit_index);
    }

    template<std::size_t N>
    class my_array<bool, N>
    {
    public:
        proxy_byte at(std::size_t index);

        bool at(std::size_t index) const;

        proxy_byte operator[](std::size_t index);

        bool operator[](std::size_t index) const;

        bool empty() const;

        std::size_t size() const;

        void fill(bool val);

    private:
        std::byte _data[(N + 7) / 8]{};
    };

    template<std::size_t N>
    proxy_byte my_array<bool, N>::at(std::size_t index)
    {
        if (index >= N)
        {
            throw std::out_of_range("Index out of range");
        }
        return proxy_byte(_data[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::at(std::size_t index) const
    {
        if (index >= N)
        {
            throw std::out_of_range("Index out of range");
        }
        return help_functions::get_bit(_data[index / 8], index % 8);
    }

    template<std::size_t N>
    proxy_byte my_array<bool, N>::operator[](std::size_t index)
    {
        return proxy_byte(_data[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::operator[](std::size_t index) const
    {
        return help_functions::get_bit(_data[index / 8], index % 8);
    }

    template<std::size_t N>
    bool my_array<bool, N>::empty() const
    {
        return N == 0;
    }

    template<std::size_t N>
    std::size_t my_array<bool, N>::size() const
    {
        return N;
    }

    template<std::size_t N>
    void my_array<bool, N>::fill(bool val)
    {
        std::byte fill_val = val ? constants::BYTE_FULL_MASK : constants::BYTE_EMPTY_MASK;
        for (std::size_t i = 0; i < N; ++i)
        {
            _data[i / 8] = fill_val;
        }
    }
}  // namespace lab_13
