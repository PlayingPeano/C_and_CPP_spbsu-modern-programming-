#include "bitstream.h"
#include "huffman.h"
#include <bitset>
#include <string>
#include <map>

namespace bitstream
{
    void write(std::string &bytes, std::ostream &out)
    {
        for (std::size_t i = 0; i < bytes.size(); i += 8)
        {
            std::bitset<8> byte{};
            for (std::size_t j = 0; j < 8; ++j)
            {
                byte[j] = bytes[i + j] == '1';
            }
            char tmp = static_cast<char>(byte.to_ulong());
            out.write(&tmp, 1);
        }
    }

    std::string &read(std::size_t codedTextSize, std::istream &in, std::map<std::string, char> &decodedMap)
    {
        static std::string decodedText;
        static std::string codedText;

        for (std::size_t i = 0; i < codedTextSize + 7; i += 8)
        {
            static std::string code;
            char tmp;
            in.read(&tmp, 1);
            std::bitset<8> byte{};
            for (std::size_t j = 0; j < 8; ++j)
            {
                byte[j] = (tmp) & (1 << j);
            }
            for (std::size_t j = 0; j < 8 && i + j < codedTextSize; ++j)
            {
                code.append(byte[j] ? "1" : "0");
                if (decodedMap.find(code) != decodedMap.end())
                {
                    char res = decodedMap[code];
                    decodedText.append(1, res);
                    code = "";
                }
            }
        }
        return decodedText;
    }
}