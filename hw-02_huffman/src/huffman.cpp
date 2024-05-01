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

    std::size_t frequency_table::GetSizeOfTable() const
    {
        return _table.size();
    }

    char node::GetValue() const
    {
        return _value;
    }

    void node::SetValue(char value)
    {
        _value = value;
    }

    std::size_t node::GetFrequency() const
    {
        return _frequency;
    }

    void node::SetFrequency(std::size_t frequency)
    {
        _frequency = frequency;
    }

    std::shared_ptr<node> node::GetLeft() const
    {
        return _left;
    }

    void node::SetLeft(std::shared_ptr<node> left)
    {
        _left = std::move(left);
    }

    std::shared_ptr<node> node::GetRight() const
    {
        return _right;
    }

    void node::SetRight(std::shared_ptr<node> right)
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
            return l->GetFrequency() > r->GetFrequency();
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
            ObtainHuffmanCodes(_root, "");
        }
    }

    void tree::ObtainHuffmanCodes(const std::shared_ptr<node> &current, const std::string &code)
    {
        if (!current)
        {
            return;
        }

        if (current->GetLeft() == nullptr && current->GetRight() == nullptr)
        {
            if (current == _root)
            {
                huffman_codes_for_bytes[current->GetValue()] = "0";
                bytes_for_huffman_codes["0"] = current->GetValue();
            } else
            {
                huffman_codes_for_bytes[current->GetValue()] = code;
                bytes_for_huffman_codes[code] = current->GetValue();
            }
            return;
        }

        ObtainHuffmanCodes(current->GetLeft(), code + "0");
        ObtainHuffmanCodes(current->GetRight(), code + "1");
    }

    std::shared_ptr<node> tree::GetRoot() const
    {
        return _root;
    }

    std::string tree::GetHuffmanCodeForByte(char value)
    {
        return huffman_codes_for_bytes[value];
    }

    char tree::GetByteForHuffmanCode(const std::string &code)
    {
        return bytes_for_huffman_codes[code];
    }

    std::map<std::string, char> &tree::GetMapBytesForHuffmanCodes()
    {
        return bytes_for_huffman_codes;
    }

    void huffman::GetDataFromFile(std::ifstream &in, std::vector<char> &data)
    {
        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);

        data.resize(fileSize);
        if (!in.read(data.data(), fileSize))
        {
            throw std::runtime_error("Can't read input file");
        }
    }

    std::pair<std::size_t, std::size_t>
    huffman::WriteCompressedDataToFile(std::ofstream &out, std::vector<char> &data)
    {
        std::size_t additionalSize = 0;
        std::size_t compressedSize = 0;
        if (data.empty())
        {
            return {additionalSize, compressedSize};
        }

        frequency_table frequencyTable(data);
        std::size_t tableSize = frequencyTable.GetSizeOfTable();
        additionalSize += sizeof(tableSize);
        out.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));

        for (const auto &[key, value]: frequencyTable._table)
        {
            if (!out.write(reinterpret_cast<const char *>(&key), sizeof(key)) ||
                !out.write(reinterpret_cast<const char *>(&value), sizeof(value)))
            {
                throw std::runtime_error("Can't write data to output file");
            }
            additionalSize += sizeof(key) + sizeof(value);
        }

        tree treeWithCodes(frequencyTable);

        std::string codedText;
        for (char ch: data)
        {
            codedText.append(treeWithCodes.GetHuffmanCodeForByte(ch));
        }

        std::size_t codedTextSize = codedText.size();
        if (!out.write(reinterpret_cast<const char *>(&codedTextSize), sizeof(codedTextSize)))
        {
            throw std::runtime_error("Can't write data to output file");
        }
        additionalSize += sizeof(codedTextSize);

        while (codedText.size() % 8 != 0)
        {
            codedText.append("0");
        }
        compressedSize = codedText.size() / 8;

        bitstream::write(codedText, out);

        return {additionalSize, compressedSize};
    }

    std::tuple<std::size_t, std::size_t, std::size_t>
    huffman::Compress(std::ifstream &in, std::ofstream &out)
    {
        std::vector<char> data;
        GetDataFromFile(in, data);

        auto [additionalSize, compressedSize] = WriteCompressedDataToFile(out, data);

        return {data.size(), compressedSize, additionalSize};
    }

    void huffman::ReadFrequencyTable(std::ifstream &in, std::map<char, std::size_t> &table)
    {
        std::size_t tableSize = 0;
        if (!in.read(reinterpret_cast<char *>(&tableSize), sizeof(tableSize)))
        {
            throw std::runtime_error("Can't read table size");
        }

        for (std::size_t i = 0; i < tableSize; ++i)
        {
            char key = 0;
            std::size_t value = 0;
            if (!in.read(reinterpret_cast<char *>(&key), sizeof(key)) ||
                !in.read(reinterpret_cast<char *>(&value), sizeof(value)))
            {
                throw std::runtime_error("Can't read table");
            }
            table[key] = value;
        }

    }

    std::size_t
    huffman::ReadEncodedDataToString(std::istream &in, std::string &data, std::map<std::string, char> &decodedMap)
    {
        std::size_t codedTextSize = 0;

        if (!in.read(reinterpret_cast<char *>(&codedTextSize), sizeof(codedTextSize)))
        {
            throw std::runtime_error("Can't read input file");
        }

        data = bitstream::read(codedTextSize, in, decodedMap);
        return (codedTextSize + 7) / 8;
    }

    std::tuple<std::size_t, std::size_t, std::size_t>
    huffman::Decompress(std::ifstream &in, std::ofstream &out)
    {
        in.seekg(0, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(0, std::ios::beg);
        if (fileSize == 0)
        {
            return {0, 0, 0};
        }

        std::map<char, std::size_t> table;
        huffman::ReadFrequencyTable(in, table);

        frequency_table frequencyTable(table);
        tree treeWithCodes(frequencyTable);

        std::string decodedText;
        std::size_t compressedDataSize = huffman::ReadEncodedDataToString(in, decodedText,
                                                                          treeWithCodes.GetMapBytesForHuffmanCodes());
        in.close();

        out.write(decodedText.c_str(), static_cast<std::streamsize>(decodedText.size()));
        out.close();

        std::size_t additionalSize = 2 * sizeof(std::size_t) + table.size() * (sizeof(std::size_t) + sizeof(char));
        return {compressedDataSize, decodedText.size(),
                additionalSize};
    }
}
