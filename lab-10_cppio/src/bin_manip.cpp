#include "bin_manip.h"
#include <iostream>
#include <fstream>
#include <exception>
#include <memory>
#include <cstring>

const std::int32_t BITS_IN_ONE_BYTE = 8;
const std::int32_t BIT_MASK_256 = 0xFF;


namespace bin_manip
{
    write_le_int32::write_le_int32(std::int32_t value) : value(value)
    {}

    std::ostream &operator<<(std::ostream &out, const write_le_int32 &manip_type)
    {
        char bytes[4];
        bytes[0] = (manip_type.value & BIT_MASK_256);
        bytes[1] = ((manip_type.value >> BITS_IN_ONE_BYTE) & BIT_MASK_256);
        bytes[2] = ((manip_type.value >> 2 * BITS_IN_ONE_BYTE) & BIT_MASK_256);
        bytes[3] = ((manip_type.value >> 3 * BITS_IN_ONE_BYTE) & BIT_MASK_256);
        out.write(bytes, 4);

        return out;
    }

    write_bool::write_bool(bool value) : value(value)
    {}

    std::ostream &operator<<(std::ostream &out, const write_bool &manip_type)
    {
        char byte = manip_type.value ? 1 : 0;
        out.write(&byte, 1);

        return out;
    }

    write_c_string::write_c_string(const char *value)
    {
        strcpy(this->value, value);
    }

    std::ostream &operator<<(std::ostream &out, const write_c_string &manip_type)
    {
        out.write(manip_type.value, strlen(manip_type.value) + 1);

        return out;
    }

    read_le_int32::read_le_int32(int32_t &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_le_int32 manip_type)
    {
        char bytes[4];
        try
        {
            in.read(bytes, 4);
        }
        catch (std::ios_base::failure &exception)
        {
            std::cerr << "Failed while reading!" << exception.what() << std::endl;
        }
        *(manip_type.value) = ((unsigned char) bytes[3] << 3 * BITS_IN_ONE_BYTE) |
                              ((unsigned char) bytes[2] << 2 * BITS_IN_ONE_BYTE) |
                              ((unsigned char) bytes[1] << BITS_IN_ONE_BYTE) | (unsigned char) bytes[0];

        return in;
    }

    read_bool::read_bool(bool &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_bool manip_type)
    {
        char byte;
        try
        {
            in.read(&byte, 1);
        }
        catch (std::ios_base::failure &exception)
        {
            std::cerr << "Failed while reading!" << exception.what() << std::endl;
        }
        *manip_type.value = byte;

        return in;
    }

    read_c_string::read_c_string(std::string &value) : value(&value)
    {}

    std::istream &operator>>(std::istream &in, read_c_string manip_type)
    {
        char ch;
        in.get(ch);
        while (ch != '\0')
        {
            *manip_type.value += ch;
            if (!in.get(ch))
            {
                throw std::invalid_argument("File doesn't contain enough data.");
            }
        }

        return in;
    }
}
