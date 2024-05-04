#include "bitstream.h"
#include "huffman.h"
#include <bitset>
#include <string>
#include <map>

namespace bitstream_help_functions
{
    std::bitset<huffman_constants::BITS_IN_ONE_BYTE> ReadOneByte(std::istream &in)
    {
        char tmp{};
        in.read(&tmp, sizeof(char));
        std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte{};
        for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
        {
            byte[j] = (tmp) & (huffman_constants::SIZE_T_ONE << j);
        }
        return byte;
    }

    bool CodeIsValid(std::string &code, std::map<std::string, char> &decodedMap, std::size_t i, std::size_t j,
                     std::size_t numOfBytesInCodedText, std::size_t sizeOfGarbageBits)
    {
        return decodedMap.find(code) != decodedMap.end() &&
               (i != numOfBytesInCodedText - 1 || j < huffman_constants::BITS_IN_ONE_BYTE - sizeOfGarbageBits);
    }
}

namespace bitstream
{
    void write(std::string &bits, std::ostream &out)
    {
        for (std::size_t i = huffman_constants::SIZE_T_ZERO; i < bits.size(); i += huffman_constants::BITS_IN_ONE_BYTE)
        {
            std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte{};
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                byte[j] = bits[i + j] == huffman_constants::CHAR_ONE;
            }
            char tmp = static_cast<char>(byte.to_ulong());
            out.write(&tmp, huffman_constants::SIZE_T_ONE);
        }
    }

    std::string &read(std::size_t codedTextSize, std::istream &in, std::map<std::string, char> &decodedMap)
    {
        static std::string decodedText;
        static std::string codedText;

        std::size_t sizeOfGarbageBits =
                (huffman_constants::BITS_IN_ONE_BYTE - (codedTextSize % huffman_constants::BITS_IN_ONE_BYTE)) %
                huffman_constants::BITS_IN_ONE_BYTE;
        std::size_t add = static_cast<int>((codedTextSize %
                                            huffman_constants::BITS_IN_ONE_BYTE) != huffman_constants::SIZE_T_ZERO);
        std::size_t numOfBytesInCodedText = codedTextSize / huffman_constants::BITS_IN_ONE_BYTE + add;

        for (std::size_t i = huffman_constants::SIZE_T_ZERO;
             i < numOfBytesInCodedText; ++i)
        {
            static std::string code{};
            std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte = bitstream_help_functions::ReadOneByte(in);
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                code.append(byte[j] ? huffman_constants::STR_ONE : huffman_constants::STR_ZERO);
                if (bitstream_help_functions::CodeIsValid(code, decodedMap, i, j, numOfBytesInCodedText,
                                                          sizeOfGarbageBits))
                {
                    char res = decodedMap[code];
                    decodedText.append(huffman_constants::SIZE_T_ONE, res);
                    code.clear();
                }
            }
        }
        return decodedText;
    }
}
