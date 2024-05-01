#include "bitstream.h"
#include "huffman.h"
#include <bitset>
#include <string>
#include <map>

namespace bitstream
{
    void write(std::string &bytes, std::ostream &out)
    {
        for (std::size_t i = huffman_constants::SIZE_T_ZERO; i < bytes.size(); i += huffman_constants::BITS_IN_ONE_BYTE)
        {
            std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte{};
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                byte[j] = bytes[i + j] == huffman_constants::CHAR_ONE;
            }
            char tmp = static_cast<char>(byte.to_ulong());
            out.write(&tmp, huffman_constants::SIZE_T_ONE);
        }
    }

    std::string &read(std::size_t codedTextSize, std::istream &in, std::map<std::string, char> &decodedMap)
    {
        static std::string decodedText;
        static std::string codedText;

        for (std::size_t i = huffman_constants::SIZE_T_ZERO; i < codedTextSize / huffman_constants::BITS_IN_ONE_BYTE +
                                    static_cast<int>(codedTextSize % huffman_constants::BITS_IN_ONE_BYTE != huffman_constants::SIZE_T_ZERO); ++i)
        {
            static std::string code{};
            char tmp{};
            in.read(&tmp, huffman_constants::SIZE_T_ONE);
            std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte{};
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                byte[j] = (tmp) & (huffman_constants::SIZE_T_ONE << j);
            }
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE &&
                                    huffman_constants::BITS_IN_ONE_BYTE * i + j < codedTextSize; ++j)
            {
                code.append(byte[j] ? huffman_constants::STR_ONE : huffman_constants::STR_ZERO);
                if (decodedMap.find(code) != decodedMap.end())
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
