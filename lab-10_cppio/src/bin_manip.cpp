#include "bin_manip.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>

const int32_t BITS_IN_ONE_BYTE = 8;
const int32_t BIT_MASK_256 = 0xFF;


namespace bin_manip
{
    write_le_int32::write_le_int32(int32_t value) : value(value)
    {}

    std::ostream &operator<<(std::ostream &out, const write_le_int32 &manip_type)
    {
        for (size_t i = 0; i < sizeof(manip_type.value); ++i)
        {
            out.put(static_cast<char>(manip_type.value >> (BITS_IN_ONE_BYTE * i) & BIT_MASK_256));
        }

        return out;
    }

    write_bool::write_bool(bool value) : value(value)
    {}

    std::ostream &operator<<(std::ostream &out, const write_bool &manip_type)
    {
        out.put(manip_type.value ? '1' : '0');

        return out;
    }

    write_c_string::write_c_string(std::string value) : value(std::move(value))
    {}

    std::ostream &operator<<(std::ostream &out, const write_c_string &manip_type)
    {
        for (unsigned char ch: manip_type.value)
        {
            out << ch;
        }


        return out;
    }

    read_le_int32::read_le_int32(int32_t &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_le_int32 manip_type)
    {
        std::int32_t x = 0;
        char ch = 0;
        for (size_t i = 0; i < 4; ++i)
        {
            if (!in.get(ch))
            {
                throw std::invalid_argument("File doesn't contain enough data.");
            }
            x |= ((unsigned char) ch & BIT_MASK_256) << (BITS_IN_ONE_BYTE * i);
        }
        *manip_type.value = x;

        return in;
    }

    read_bool::read_bool(bool &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_bool manip_type)
    {
        char ch;
        if (!in.get(ch))
        {
            throw std::invalid_argument("File doesn't contain enough data.");
        }
        *manip_type.value = ch & 1;

        return in;
    }

    read_c_string::read_c_string(std::string &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_c_string manip_type)
    {
        if (!getline(in, *manip_type.value))
        {
            throw std::invalid_argument("File doesn't contain enough data.");
        }
//        char ch;
//        in.get(ch);
//        while (ch != '\0')
//        {
//            *manip_type.value += ch;
//            if (!in.get(ch))
//            {
//                throw std::invalid_argument("File doesn't contain enough data.");
//            }
//        }

        return in;
    }
}
