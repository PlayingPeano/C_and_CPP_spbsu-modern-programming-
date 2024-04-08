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
            std::string reg;
            std::cin >> reg;

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

            if (fin.eof())
            {
                std::cout << "LOAD: empty file." << std::endl;
                continue;
            }
            fin >> rows;

            if (fin.eof())
            {
                std::cout << "LOAD: empty file." << std::endl;
                continue;
            }
            fin >> cols;

            try
            {
                matrix::Matrix mat(rows, cols);
                fin >> mat;
                matrices[std::stoi(&reg[1])] = mat;
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "LOAD: " << e.what() << std::endl;
            }
        }
        else if (input == "print")
        {
            std::string reg;
            std::cin >> reg;

            std::cout << matrices[std::stoi(&reg[1])];
        }
        else if (input == "add")
        {
            std::string reg;
            std::cin >> reg;

            std::string reg2;
            std::cin >> reg2;

            try
            {
                matrices[std::stoi(&reg[1])] += matrices[std::stoi(&reg2[1])];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "ADD: " << e.what() << std::endl;
            }
        }
        else if (input == "mul")
        {
            std::string reg1;
            std::cin >> reg1;

            std::string reg2;
            std::cin >> reg2;

            try
            {
                matrices[std::stoi(&reg1[1])] *= matrices[std::stoi(&reg2[1])];
            }
            catch (matrix::MatrixException &e)
            {
                std::cout << "MUL: " << e.what() << std::endl;
            }
        }
        else if (input == "elem")
        {
            std::string reg;
            std::cin >> reg;

            std::size_t row;
            std::cin >> row;

            std::size_t col;
            std::cin >> col;

            try
            {
                int tmp = matrices[std::stoi(&reg[1])](row, col);
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