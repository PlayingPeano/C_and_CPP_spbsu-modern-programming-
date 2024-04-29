#include "huffman.h"
#include <array>
#include <algorithm>
#include <vector>
#include <utility>
#include <queue>
#include <string>
#include <fstream>
#include <tuple>
#include <filesystem>
#include "bitstream.h"


namespace huffman_compression
{
    frequency_table::frequency_table(std::map<char, std::size_t> table)
    {
        _table = std::move(table);
    }

    frequency_table::frequency_table(const std::vector<char> &bytes)
    {
        for (auto &byte: bytes)
        {
            ++_table[byte];
        }
    }

    std::size_t frequency_table::operator[](char value)
    {
        return _table[value];
    }

    std::size_t frequency_table::get_size_of_table() const
    {
        return _table.size();
    }

    const std::map<char, std::size_t> &frequency_table::get_table() const
    {
        return _table;
    }

    char node::get_value() const
    {
        return _value;
    }

    void node::set_value(char value)
    {
        _value = value;
    }

    std::size_t node::get_frequency() const
    {
        return _frequency;
    }

    void node::set_frequency(std::size_t frequency)
    {
        _frequency = frequency;
    }

    std::shared_ptr<node> node::get_left() const
    {
        return _left;
    }

    void node::set_left(std::shared_ptr<node> left)
    {
        _left = std::move(left);
    }

    std::shared_ptr<node> node::get_right() const
    {
        return _right;
    }

    void node::set_right(std::shared_ptr<node> right)
    {
        _right = std::move(right);
    }

    tree::tree()
    {
        _root = nullptr;
    }

    tree::tree(const frequency_table &table) : tree()
    {
        auto cmp_greater_priority_node = [](const std::shared_ptr<node> &l, const std::shared_ptr<node> &r)
        {
            return l->get_frequency() > r->get_frequency();
        };

        std::priority_queue<std::shared_ptr<node>, std::vector<std::shared_ptr<node >>,
                decltype(cmp_greater_priority_node)> pq(cmp_greater_priority_node);

        for (const auto &[key, value]: table._table)
        {
            pq.push(std::make_unique<node>(key, value));
        }

        while (pq.size() > 1)
        {
            auto left = pq.top();
            pq.pop();
            auto right = pq.top();
            pq.pop();

            pq.push(std::make_unique<node>(std::move(left), std::move(right)));
        }

        if (!pq.empty())
        {
            _root = pq.top();
            pq.pop();
            obtain_huffman_codes(_root, "");
        }
    }

    void tree::obtain_huffman_codes(const std::shared_ptr<node> &current, const std::string &code)
    {
        if (!current)
        {
            return;
        }

        if (current->get_left() == nullptr && current->get_right() == nullptr)
        {
            if (current == _root)
            {
                huffman_codes_for_bytes[current->get_value()] = "0";
                bytes_for_huffman_codes["0"] = current->get_value();
            } else
            {
                huffman_codes_for_bytes[current->get_value()] = code;
                bytes_for_huffman_codes[code] = current->get_value();
            }
            return;
        }

        obtain_huffman_codes(current->get_left(), code + "0");
        obtain_huffman_codes(current->get_right(), code + "1");
    }

    std::shared_ptr<node> tree::get_root() const
    {
        return _root;
    }

    std::string tree::get_huffman_code_for_byte(char value)
    {
        return huffman_codes_for_bytes[value];
    }

    char tree::get_byte_for_huffman_code(const std::string &code)
    {
        return bytes_for_huffman_codes[code];
    }

    std::map<std::string, char> &tree::get_map_bytes_for_huffman_codes()
    {
        return bytes_for_huffman_codes;
    }

    std::tuple<std::size_t, std::size_t, std::size_t>
    huffman::compress(std::string &in_filename, std::string &out_filename)
    {
        std::tuple<std::size_t, std::size_t, std::size_t> result = {0, 0, 0};
        std::ifstream in(in_filename, std::ios::binary);

        if (!in.is_open())
        {
            throw std::runtime_error("Can't open input file");
        }

        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);

        std::vector<char> data(fileSize);

        if (!in.read(data.data(), fileSize))
        {
            throw std::runtime_error("Can't read input file");
        }
        in.close();

        frequency_table frequencyTable(data);
        tree treeWithCodes(frequencyTable);

        std::ofstream out(out_filename, std::ios::binary);
        if (!out.is_open())
        {
            throw std::runtime_error("Can't open output file");
        }

        std::size_t tableSize = frequencyTable.get_size_of_table();

        if (tableSize == 0)
        {
            out.close();
            return {0, 0, 0};
        }
        out.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));
        std::get<2>(result) = sizeof(tableSize);


        for (const auto &[key, value]: frequencyTable.get_table())
        {
            out.write(&key, sizeof(key));
            out.write(reinterpret_cast<const char *>(&value), sizeof(value));
            std::get<2>(result) += sizeof(key) + sizeof(value);
        }

        std::string coded_text;
        for (char ch: data)
        {
            coded_text.append(treeWithCodes.get_huffman_code_for_byte(ch));
        }

        std::size_t codedTextSize = coded_text.size();
        out.write(reinterpret_cast<const char *>(&codedTextSize), sizeof(codedTextSize));
        std::get<2>(result) += sizeof(codedTextSize);

        while (coded_text.size() % 8 != 0)
        {
            coded_text.append("0");
        }
        std::get<1>(result) = coded_text.size() / 8;

        bitstream::write(coded_text, out);
        out.close();

        std::get<0>(result) = data.size();

        return result;
    }

    std::tuple<std::size_t, std::size_t, std::size_t>
    huffman::decompress(std::string &in_filename, std::string &out_filename)
    {
        std::tuple<std::size_t, std::size_t, std::size_t> result = {0, 0, 0};
        std::ifstream in(in_filename, std::ios::binary);

        if (!in.is_open())
        {
            throw std::runtime_error("Can't open input file");
        }

        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);
        std::size_t tableSize = 0;
        if (fileSize)
        {
            if (!in.read(reinterpret_cast<char *>(&tableSize), sizeof(tableSize)))
            {
                throw std::runtime_error("Can't read input file");
            }
            std::get<2>(result) = sizeof(tableSize);
        }

        std::map<char, std::size_t> table;
        for (std::size_t i = 0; i < tableSize; ++i)
        {
            char ch = 0;
            std::size_t frequency = 0;
            if (!in.read(&ch, sizeof(ch)) || !in.read(reinterpret_cast<char *>(&frequency), sizeof(frequency)))
            {
                throw std::runtime_error("Can't read input file");
            }

            table[ch] = frequency;

            std::get<2>(result) += sizeof(ch) + sizeof(frequency);
        }

        frequency_table frequencyTable(table);
        tree treeWithCodes(frequencyTable);

        std::size_t codedTextSize = 0;
        if (fileSize)
        {
            if (!in.read(reinterpret_cast<char *>(&codedTextSize), sizeof(codedTextSize)))
            {
                throw std::runtime_error("Can't read input file");
            }
            std::get<2>(result) += sizeof(codedTextSize);
            std::get<0>(result) = (codedTextSize + 7) / 8;
        }
        std::string decodedText = bitstream::read(codedTextSize, in, treeWithCodes.get_map_bytes_for_huffman_codes());
        in.close();

        std::ofstream out(out_filename, std::ios::binary);
        if (!out.is_open())
        {
            throw std::runtime_error("Can't open output file");
        }

        out.write(decodedText.c_str(), static_cast<std::streamsize>(decodedText.size()));
        out.close();
        std::get<1>(result) = decodedText.size();

        return result;
    }
}
