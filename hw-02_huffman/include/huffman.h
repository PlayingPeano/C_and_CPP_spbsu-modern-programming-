#pragma once

#include <iostream>
#include <cstddef>
#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <string>
#include <tuple>
#include <filesystem>

namespace huffman_constants
{
    const std::size_t NUMBER_OF_POSSIBLE_BYTES = 256;
    const int ABS_OF_MIN_CHAR = 128;
}

namespace huffman_compression
{
    class huffman
    {
    public:
        static std::tuple<std::size_t, std::size_t, std::size_t> compress(std::string &in_filename, std::string &out_filename);

        static std::tuple<std::size_t, std::size_t, std::size_t> decompress(std::string &in_filename, std::string &out_filename);
    };

    struct frequency_table
    {
        std::map<char, std::size_t> _table;

        frequency_table() = default;

        explicit frequency_table(std::map<char, std::size_t> table);

        explicit frequency_table(const std::vector<char> &bytes);

        std::size_t operator[](char value);

        std::size_t get_size_of_table() const;

        const std::map<char, std::size_t> &get_table() const;
    };

    class node
    {
    private:
        char _value;
        std::size_t _frequency;

        std::shared_ptr <node> _left;
        std::shared_ptr <node> _right;

    public:
        node() : _value(0), _frequency(0), _left(nullptr), _right(nullptr)
        {}

        node(char value, std::size_t frequency) : _value(value), _frequency(frequency), _left(nullptr),
                                                  _right(nullptr)
        {}

        node(std::shared_ptr <node> left,
             std::shared_ptr <node> right) : _value(0), _frequency(left->_frequency + right->_frequency),
                                             _left(std::move(left)),
                                             _right(std::move(right))
        {}

        char get_value() const;

        void set_value(char value);

        std::size_t get_frequency() const;

        void set_frequency(std::size_t frequency);

        std::shared_ptr <node> get_left() const;

        void set_left(std::shared_ptr <node> left);

        std::shared_ptr <node> get_right() const;

        void set_right(std::shared_ptr <node> right);
    };

    class tree
    {
    private:
        std::shared_ptr <node> _root = nullptr;
        std::map<char, std::string> huffman_codes_for_bytes;
        std::map<std::string, char> bytes_for_huffman_codes;

        void obtain_huffman_codes(const std::shared_ptr <node> &node, const std::string &code);

    public:
        tree();

        explicit tree(const frequency_table &table);

        std::shared_ptr <node> get_root() const;

        std::string get_huffman_code_for_byte(char value);

        char get_byte_for_huffman_code(const std::string &code);
    };
}
