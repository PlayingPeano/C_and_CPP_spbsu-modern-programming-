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

namespace huffman_exceptions
{
    HuffmanException::HuffmanException(std::string message) : _message(std::move(message))
    {}

    const char *HuffmanException::what() const noexcept
    {
        return _message.c_str();
    }
}

namespace huffman_help_functions
{
    std::size_t GetSizeOfFile(std::ifstream &in)
    {
        in.seekg(huffman_constants::SIZE_T_ZERO, std::ios::end);
        std::streampos fileSize = in.tellg();
        in.seekg(huffman_constants::SIZE_T_ZERO, std::ios::beg);
        return static_cast<std::size_t>(fileSize);
    }

    void GetDataFromFile(std::ifstream &in, std::vector<char> &data)
    {
        std::size_t fileSize = GetSizeOfFile(in);
        data.resize(fileSize);
        if (!in.read(data.data(), static_cast<std::streamsize>(fileSize)))
        {
            throw huffman_exceptions::HuffmanException("Can't read input file");
        }
    }

    std::size_t
    WriteAdditionalDataToFile(std::ofstream &out, std::vector<char> &data, huffman_compression::tree &treeWithCodes)
    {
        std::size_t additionalSize = huffman_constants::SIZE_T_ZERO;
        if (data.empty())
        {
            return additionalSize;
        }

        huffman_compression::frequency_table frequencyTable(data);
        std::size_t tableSize = frequencyTable.GetSizeOfTable();
        additionalSize += sizeof(tableSize);
        out.write(reinterpret_cast<const char *>(&tableSize), sizeof(tableSize));

        for (const auto &[key, value]: frequencyTable._table)
        {
            if (!out.write(reinterpret_cast<const char *>(&key), sizeof(key)) ||
                !out.write(reinterpret_cast<const char *>(&value), sizeof(value)))
            {
                throw huffman_exceptions::HuffmanException("Can't write data to output file");
            }
            additionalSize += sizeof(key) + sizeof(value);
        }

        treeWithCodes = huffman_compression::tree(frequencyTable);
        return additionalSize;
    }

    std::pair<std::size_t, std::size_t>
    WriteCompressedDataToFile(std::ofstream &out, std::vector<char> &data, huffman_compression::tree &treeWithCodes)
    {
        std::size_t additionalSize = huffman_constants::SIZE_T_ZERO;
        std::size_t compressedSize = huffman_constants::SIZE_T_ZERO;

        if (data.empty())
        {
            return {additionalSize, compressedSize};
        }

        std::string codedText;
        for (char ch: data)
        {
            codedText.append(treeWithCodes.GetHuffmanCodeForByte(ch));
        }

        std::size_t codedTextSize = codedText.size();
        if (!out.write(reinterpret_cast<const char *>(&codedTextSize), sizeof(codedTextSize)))
        {
            throw huffman_exceptions::HuffmanException("Can't write data to output file");
        }
        additionalSize += sizeof(codedTextSize);

        while (codedText.size() % huffman_constants::BITS_IN_ONE_BYTE != huffman_constants::SIZE_T_ZERO)
        {
            codedText.append(huffman_constants::STR_ZERO);
        }
        compressedSize = codedText.size() / huffman_constants::BITS_IN_ONE_BYTE;

        bitstream::write(codedText, out);

        return {additionalSize, compressedSize};
    }

    void ReadFrequencyTable(std::ifstream &in, std::map<char, std::size_t> &table)
    {
        std::size_t tableSize = huffman_constants::SIZE_T_ZERO;
        if (!in.read(reinterpret_cast<char *>(&tableSize), sizeof(tableSize)))
        {
            throw huffman_exceptions::HuffmanException("Can't read table size");
        }

        for (std::size_t i = huffman_constants::SIZE_T_ZERO; i < tableSize; ++i)
        {
            char key{};
            std::size_t value = huffman_constants::SIZE_T_ZERO;
            if (!in.read(reinterpret_cast<char *>(&key), sizeof(key)) ||
                !in.read(reinterpret_cast<char *>(&value), sizeof(value)))
            {
                throw huffman_exceptions::HuffmanException("Can't read table");
            }
            table[key] = value;
        }
    }

    std::size_t
    ReadEncodedDataToString(std::istream &in, std::string &data, std::map<std::string, char> &decodedMap)
    {
        std::size_t codedTextSize = huffman_constants::SIZE_T_ZERO;

        if (!in.read(reinterpret_cast<char *>(&codedTextSize), sizeof(codedTextSize)))
        {
            throw huffman_exceptions::HuffmanException("Can't read input file");
        }

        data = bitstream::read(codedTextSize, in, decodedMap);
        return codedTextSize / huffman_constants::BITS_IN_ONE_BYTE +
               static_cast<int>(codedTextSize % huffman_constants::BITS_IN_ONE_BYTE != huffman_constants::SIZE_T_ZERO);
    }
}

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

    std::size_t node::GetFrequency() const
    {
        return _frequency;
    }

    std::shared_ptr<node> node::GetLeft() const
    {
        return _left;
    }

    std::shared_ptr<node> node::GetRight() const
    {
        return _right;
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
            ObtainHuffmanCodes(_root, std::string{});
        }
    }

    void tree::swap(tree &first, tree &second)
    {
        std::swap(first._root, second._root);
        std::swap(first.huffman_codes_for_bytes, second.huffman_codes_for_bytes);
        std::swap(first.bytes_for_huffman_codes, second.bytes_for_huffman_codes);
    }

    tree &tree::operator=(tree &&tree) noexcept
    {
        if (this != &tree)
        {
            swap(*this, tree);
        }
        return *this;
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
                huffman_codes_for_bytes[current->GetValue()] = huffman_constants::STR_ZERO;
                bytes_for_huffman_codes[huffman_constants::STR_ZERO] = current->GetValue();
            } else
            {
                huffman_codes_for_bytes[current->GetValue()] = code;
                bytes_for_huffman_codes[code] = current->GetValue();
            }
            return;
        }

        ObtainHuffmanCodes(current->GetLeft(), code + huffman_constants::STR_ZERO);
        ObtainHuffmanCodes(current->GetRight(), code + huffman_constants::STR_ONE);
    }


    std::shared_ptr<node> tree::GetRoot() const
    {
        return _root;
    }

    std::string tree::GetHuffmanCodeForByte(char value)
    {
        if (huffman_codes_for_bytes.find(value) == huffman_codes_for_bytes.end())
        {
            return huffman_constants::STR_EMPTY;
        }
        return huffman_codes_for_bytes[value];
    }

    char tree::GetByteForHuffmanCode(const std::string &code)
    {
        if (bytes_for_huffman_codes.find(code) == bytes_for_huffman_codes.end())
        {
            return huffman_constants::CHAR_EMPTY;
        }
        return bytes_for_huffman_codes[code];
    }

    std::map<std::string, char> &tree::GetMapBytesForHuffmanCodes()
    {
        return bytes_for_huffman_codes;
    }


    std::tuple<std::size_t, std::size_t, std::size_t>
    Compress(std::ifstream &in, std::ofstream &out)
    {
        std::vector<char> data{};
        huffman_help_functions::GetDataFromFile(in, data);

        tree treeWithCodes;
        std::size_t additionalSize = huffman_help_functions::WriteAdditionalDataToFile(out, data, treeWithCodes);

        auto [addOns, compressedSize] = huffman_help_functions::WriteCompressedDataToFile(out, data, treeWithCodes);

        return {data.size(), compressedSize, additionalSize + addOns};
    }

    std::tuple<std::size_t, std::size_t, std::size_t>
    Decompress(std::ifstream &in, std::ofstream &out)
    {
        std::size_t fileSize = huffman_help_functions::GetSizeOfFile(in);
        if (fileSize == huffman_constants::SIZE_T_ZERO)
        {
            return {huffman_constants::SIZE_T_ZERO, huffman_constants::SIZE_T_ZERO, huffman_constants::SIZE_T_ZERO};
        }

        std::map<char, std::size_t> table;
        huffman_help_functions::ReadFrequencyTable(in, table);

        frequency_table frequencyTable(table);
        tree treeWithCodes(frequencyTable);

        std::string decodedText;
        std::size_t compressedDataSize = huffman_help_functions::ReadEncodedDataToString(in, decodedText,
                                                                                         treeWithCodes.GetMapBytesForHuffmanCodes());

        out.write(decodedText.c_str(), static_cast<std::streamsize>(decodedText.size()));

        std::size_t additionalSize =
                sizeof(std::size_t) + sizeof(std::size_t) + table.size() * (sizeof(std::size_t) + sizeof(char));
        return {compressedDataSize, decodedText.size(),
                additionalSize};
    }
}
