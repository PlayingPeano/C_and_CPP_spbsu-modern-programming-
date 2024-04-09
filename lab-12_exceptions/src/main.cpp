#include "matrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

const int NUMBER_OF_MATRICES = 10;

int main(int argc, char *argv[])
{
    matrix::Matrix matrices[NUMBER_OF_MATRICES];

    while (1)
    {
        std::string input;
        std::cin >> input;

        if (input == "load")
        {
            char r;
            int reg;
            std::cin >> r >> reg;

            std::string filename;
            std::cin >> filename;

            std::ifstream fin(filename);
            if (!fin.is_open())
            {
                std::cout << "LOAD: unable to open file." << std::endl;
                continue;
            }

            std::size_t rows = 0;
            std::size_t cols = 0;

            fin >> rows;
            fin >> cols;

            try
            {
                matrix::Matrix mat(rows, cols);
                fin >> mat;
                matrices[reg] = mat;
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "LOAD: " << e.what() << std::endl;
            }
            catch (std::bad_alloc &)
            {
                std::cout << "Unable to allocate memory." << std::endl;
            }
        }
        else if (input == "print")
        {
            char r;
            int reg;
            std::cin >> r >> reg;

            std::cout << matrices[reg];
        }
        else if (input == "add")
        {
            char r;
            int reg1;
            std::cin >> r >> reg1;

            int reg2;
            std::cin >> r >> reg2;

            try
            {
                matrices[reg1] += matrices[reg2];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "ADD: " << e.what() << std::endl;
            }
        }
        else if (input == "mul")
        {
            char r;
            int reg1;
            std::cin >> r >> reg1;

            int reg2;
            std::cin >> r >> reg2;

            try
            {
                matrices[reg1] *= matrices[reg2];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "MUL: " << e.what() << std::endl;
            }
            catch (std::bad_alloc &)
            {
                std::cout << "Unable to allocate memory." << std::endl;
            }
        }
        else if (input == "elem")
        {
            char r;
            int reg;
            std::cin >> r >> reg;

            std::size_t row;
            std::cin >> row;

            std::size_t col;
            std::cin >> col;

            try
            {
                int tmp = matrices[reg](row, col);
                std::cout << tmp << std::endl;
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "ACCESS: " << e.what() << std::endl;
            }
        }
        else if (input == "exit")
        {
            return 0;
        }
    }

}