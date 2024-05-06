#include <fstream>
#include <iostream>
#include <exception>
#include <tuple>
#include "huffman.h"
#include "parser.h"

int main(int argc, char **argv)
{
    try
    {
        huffman_ui::ProcessInput(argc, argv);
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}
