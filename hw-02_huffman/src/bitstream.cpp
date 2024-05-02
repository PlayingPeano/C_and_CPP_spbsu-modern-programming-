#include "bitstream.h"
#include "huffman.h"
#include <bitset>
#include <string>
#include <map>

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

//        std::size_t sizeOfGarbageBits =
//                (huffman_constants::BITS_IN_ONE_BYTE - (codedTextSize % huffman_constants::BITS_IN_ONE_BYTE)) %
//                huffman_constants::BITS_IN_ONE_BYTE;

        std::size_t add = static_cast<int>((codedTextSize %
                                           huffman_constants::BITS_IN_ONE_BYTE) !=
                                           huffman_constants::SIZE_T_ZERO);

        std::size_t numOfBytesInCodedText = codedTextSize / huffman_constants::BITS_IN_ONE_BYTE + add;

//        std::size_t garbageChars = huffman_constants::SIZE_T_ZERO;
        for (std::size_t i = huffman_constants::SIZE_T_ZERO;
             i < numOfBytesInCodedText; ++i)
        {
            static std::string code{};
            char tmp{};
            in.read(&tmp, sizeof(char));
            std::bitset<huffman_constants::BITS_IN_ONE_BYTE> byte{};
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                byte[j] = (tmp) & (huffman_constants::SIZE_T_ONE << j);
            }
            for (std::size_t j = huffman_constants::SIZE_T_ZERO; j < huffman_constants::BITS_IN_ONE_BYTE; ++j)
            {
                code.append(byte[j] ? huffman_constants::STR_ONE : huffman_constants::STR_ZERO);
                if (i == numOfBytesInCodedText - 1 && j == sizeOfGarbageBits)
                {
                    code.clear();
                }
                if (decodedMap.find(code) != decodedMap.end())
                {
//                    if (i == numOfBytesInCodedText - 1 && j < sizeOfGarbageBits)
//                    {
//                        ++garbageChars;
//                    }
                    char res = decodedMap[code];
                    decodedText.append(huffman_constants::SIZE_T_ONE, res);
                    code.clear();
                }
            }

//            while (garbageChars)
//            {
//                --garbageChars;
//                decodedText.pop_back();
//            }
        }
        return decodedText;
    }
}
