#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

namespace bin_manip
{
    class write_le_int32
    {
    private:
        std::int32_t value;
    public:
        explicit write_le_int32(std::int32_t value);

        friend std::ostream &operator<<(std::ostream &out, const write_le_int32 &manip_type);
    };

    class write_bool
    {
    private:
        bool value;
    public:
        explicit write_bool(bool value);

        friend std::ostream &operator<<(std::ostream &out, const write_bool &manip_type);
    };

    class write_c_string
    {
    private:
        std::string value;
    public:
        explicit write_c_string(std::string value);

        friend std::ostream &operator<<(std::ostream &out, const write_c_string &manip_type);
    };

    class read_le_int32
    {
    private:
        std::int32_t *value;
    public:
        explicit read_le_int32(std::int32_t &value);

        friend std::istream &operator>>(std::istream &in, read_le_int32 manip_type);
    };

    class read_bool
    {
    private:
        bool *value;
    public:
        explicit read_bool(bool &value);

        friend std::istream &operator>>(std::istream &in, read_bool manip_type);
    };

    class read_c_string
    {
    private:
        std::string *value;
    public:
        explicit read_c_string(std::string &value);

        friend std::istream &operator>>(std::istream &in, read_c_string manip_type);
    };
}