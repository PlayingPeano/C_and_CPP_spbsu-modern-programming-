#include <fstream>
#include <iostream>
#include <exception>
#include <tuple>
#include "huffman.h"

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        std::cerr << "Wrong args, check documentation" << std::endl;
        return -1;
    }

    std::string action;
    std::string InFile;
    std::string OutFile;

    for (std::size_t i = 1; i < 6; ++i)
    {
        std::string s = std::string(argv[i]);

        if (s == "-u" || s == "-c")
        {
            action = s;
        } else if (s == "-f" && i < 5)
        {
            InFile = std::string(argv[i++ + 1]);
        } else if (s == "-o" && i < 5)
        {
            OutFile = std::string(argv[i++ + 1]);
        } else
        {
            return -1;
        }
    }

    if (action == "-c")
    {
        try
        {
            std::ifstream in(InFile, std::ios::binary);
            if (!in.is_open())
            {
                std::cerr << "Can't open input file" << std::endl;
                return -1;
            }

            std::ofstream out(OutFile, std::ios::binary | std::ios::trunc);
            if(!out.is_open())
            {
                std::cerr << "Can't open output file" << std::endl;
                return -1;
            }

            auto [sizeOfInputFile, pureSizeOfResult, sizeOfAdditionalData] = huffman_compression::huffman::Compress(
                    in,
                    out);
            std::cout << sizeOfInputFile << std::endl << pureSizeOfResult << std::endl << sizeOfAdditionalData
                      << std::endl;
            in.close();
            out.close();
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
            std::ifstream in(InFile, std::ios::binary);
            if (!in.is_open())
            {
                std::cerr << "Can't open input file" << std::endl;
                return -1;
            }
            std::ofstream out(OutFile, std::ios::binary);
            if(!out.is_open())
            {
                std::cerr << "Can't open output file" << std::endl;
                return -1;
            }

            auto [sizeOfInputFile, pureSizeOfResult, sizeOfAdditionalData] = huffman_compression::huffman::Decompress(
                    in,
                    out);
            std::cout << sizeOfInputFile << std::endl << pureSizeOfResult << std::endl << sizeOfAdditionalData
                      << std::endl;
            in.close();
            out.close();
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << exception.what() << std::endl;
            return -1;
        }
    }
}
