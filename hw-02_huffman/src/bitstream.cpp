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
            char byte = 0;
            for (std::size_t j = 0; j < 8; ++j)
            {
                byte |= ((bytes[i + j] - '0') << (7 - j));
            }
            out.write(&byte, 1);
        }
    }

    std::string &read(std::size_t codedTextSize, std::istream &in, std::map<std::string, char> &decodedMap)
    {
        static std::string decodedText;
        static std::string codedText;

        for (std::size_t i = 0; i < codedTextSize; i += 8)
        {
            static std::string code;
            char tmp;
            in.read(&tmp, 1);
            auto byte = std::bitset<8>(tmp).to_string();
            for (std::size_t j = 0; j < 8 && i + j < codedTextSize; ++j)
            {
                code.append(byte[j] == '0' ? "0" : "1");
                if (decodedMap[code])
                {
                    decodedText.append(&decodedMap[code]);
                    code = "";
                }
            }
        }
        return decodedText;
    }
}