#include <fstream>
#include <iostream>
#include <exception>
#include "huffman.h"

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        std::cerr << "Wrong args, check documentation" << std::endl;
        return -1;
    }

    std::string action;
    std::string in_file;
    std::string out_file;

    for (std::size_t i = 1; i < 6; ++i)
    {
        std::string s = std::string(argv[i]);

        if (s == "-u" || s == "-c")
        {
            action = s;
        } else if (s == "-f" && i < 5)
        {
            in_file = std::string(argv[i++ + 1]);
        } else if (s == "-o" && i < 5)
        {
            out_file = std::string(argv[i++ + 1]);
        } else
        {
            return -1;
        }
    }

    if (action == "-c")
    {
        try
        {
            huffman_compression::huffman::compress(in_file, out_file);
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << exception.what() << std::endl;
            return -1;
        }
    } else
    {
        try
        {
            huffman_compression::huffman::compress(in_file, out_file);
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << exception.what() << std::endl;
            return -1;
        }
    }
}
