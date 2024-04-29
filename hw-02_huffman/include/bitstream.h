#pragma once
#include <iostream>
#include <string>
#include <map>

namespace bitstream {
    void write(std::string &bytes, std::ostream &out);
    std::string &read(std::size_t codedTextSize, std::istream &in, std::map<std::string, char> &decodedMap);
}
