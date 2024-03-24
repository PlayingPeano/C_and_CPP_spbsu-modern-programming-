#include "employees.h"
#include "bin_manip.h"
#include <iostream>
#include <fstream>
#include <memory>


int main(int argc, char *argv[])
{
    auto *employee_array = new Employee::EmployeesArray();
    std::string input;
    bool stop = false;

    while (!stop)
    {
        std::cin >> input;

        if (input == "exit")
            stop = true;
        else if (input == "load")
        {
            std::string fileName;
            if (!(std::cin >> fileName))
            {
                throw std::invalid_argument("No file name was specified.");
            }
            std::ifstream in;
            in.open(fileName, std::ios::binary);
            in >> *employee_array;
        }
        else if (input == "save")
        {
            std::string fileName;
            if (!(std::cin >> fileName))
            {
                throw std::invalid_argument("No file name was specified.");
            }
            std::ofstream out;
            out.open(fileName, std::ios::binary);
            out << *employee_array;
        }
        else if (input == "add")
        {
            std::cin >> *employee_array;
        }
        else if (input == "list")
        {
            std::cout << *employee_array;
        }
    }
    return EXIT_SUCCESS;
}