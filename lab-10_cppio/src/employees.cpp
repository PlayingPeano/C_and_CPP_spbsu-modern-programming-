#include <utility>
#include "employees.h"
#include "bin_manip.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>

namespace Employee
{
    Employee::Employee(std::string name, std::int32_t base_salary) : _name(std::move(name)), _base_salary(base_salary)
    {}

    std::ostream &operator<<(std::ostream &out, const Employee &employee)
    {
        employee.write_text(out);
        return out;
    }

    std::istream &operator>>(std::istream &in, Employee &employee)
    {
        std::string name;
        int32_t base_salary;

        in >> name;
        if (in.fail())
        {
            throw std::invalid_argument("No name was specified.");
        }
        in >> base_salary;
        if (in.fail())
        {

            throw std::invalid_argument("No base salary was specified.");
        }

        employee.read_text(in);

        employee._base_salary = base_salary;
        employee._name = name;

        return in;
    }

    std::ofstream &operator<<(std::ofstream &out, const Employee &employee)
    {
        employee.write_bin(out);
        return out;
    }

    std::ifstream &operator>>(std::ifstream &in, Employee &employee)
    {
        employee.read_bin(in);
        return in;
    }


    Employee::~Employee() = default;

    Developer::Developer(std::string name, std::int32_t base_salary, bool has_bonus) : Employee(std::move(name),
                                                                                                base_salary),
                                                                                       _has_bonus(has_bonus)
    {}

    std::int32_t Developer::salary() const
    {
        return _base_salary + 1000 * (_has_bonus ? 1 : 0);
    }

    void Developer::read_text(std::istream &in)
    {
        bool has_bonus;

        in >> has_bonus;
        if (in.fail())
        {
            throw std::invalid_argument("No bonus was specified.");
        }

        _has_bonus = has_bonus;
    }

    void Developer::write_text(std::ostream &out) const
    {
        out << "Developer" << std::endl << "Name: " << _name << '\0' << std::endl << "Base Salary: " << _base_salary
            << std::endl << "Has bonus: " << (_has_bonus ? "+" : "-") << std::endl;
    }

    void Developer::read_bin(std::ifstream &in)
    {
        in >> bin_manip::read_c_string(_name) >> bin_manip::read_le_int32(_base_salary)
           >> bin_manip::read_bool(_has_bonus);
    }

    void Developer::write_bin(std::ofstream &out) const
    {
        out << bin_manip::write_le_int32(1) << bin_manip::write_c_string(_name)
            << bin_manip::write_le_int32(_base_salary) << bin_manip::write_bool(_has_bonus);
    }

    SalesManager::SalesManager(std::string name, std::int32_t base_salary, std::int32_t sold_nm,
                               std::int32_t price)
            : Employee(std::move(name),
                       base_salary),
              _sold_nm(sold_nm),
              _price(price)
    {}

    std::int32_t SalesManager::salary() const
    {
        return _base_salary + static_cast<std::int32_t>(_sold_nm * _price * 0.01);
    }

    void SalesManager::read_text(std::istream &in)
    {
        std::int32_t sold_nm;
        std::int32_t price;

        in >> sold_nm;
        if (in.fail())
        {
            throw std::invalid_argument("No sold was specified.");
        }

        in >> price;
        if (in.fail())
        {
            throw std::invalid_argument("No price was specified.");
        }
        _sold_nm = sold_nm;
        _price = price;
    }

    void SalesManager::write_text(std::ostream &out) const
    {
        out << "Sales Manager" << std::endl << "Name: " << _name << std::endl << "Base Salary: " << _base_salary
            << std::endl << "Sold items: " << _sold_nm << std::endl
            << "Item price: " << _price << std::endl;
    }

    void SalesManager::read_bin(std::ifstream &in)
    {
        in >> bin_manip::read_c_string(_name) >> bin_manip::read_le_int32(_base_salary)
           >> bin_manip::read_le_int32(_sold_nm) >> bin_manip::read_le_int32(_price);
    }

    void SalesManager::write_bin(std::ofstream &out) const
    {
        out << bin_manip::write_le_int32(2) << bin_manip::write_c_string(_name)
            << bin_manip::write_le_int32(_base_salary)
            << bin_manip::write_le_int32(_sold_nm) << bin_manip::write_le_int32(_price);
    }

    EmployeesArray::EmployeesArray()
    {
        _employees = std::vector<Employee*>();
    }

    EmployeesArray::~EmployeesArray()
    {
        for (auto &employeePtr: _employees)
        {
            delete employeePtr;
        }
    }

    std::int32_t EmployeesArray::total_salary() const
    {
        std::int32_t total = 0;
        for (const auto &employee: _employees)
        {
            total += employee->salary();
        }
        return total;
    }

    std::ostream &operator<<(std::ostream &out, const EmployeesArray &employees_array)
    {
        int num = 1;
        for (const auto &employee: employees_array._employees)
        {
            out << num++ << ". " << *(employee);
        }

        out << "== Total salary: " << employees_array.total_salary() << std::endl << std::endl;
        return out;
    }

    std::ofstream &operator<<(std::ofstream &out, const EmployeesArray &employees_array)
    {
        out << bin_manip::write_le_int32(static_cast<std::int32_t>(employees_array._employees.size()));
        for (const auto &employee: employees_array._employees)
        {
            out << *employee;
        }
        return out;
    }

    std::istream &operator>>(std::istream &in, EmployeesArray &employees_array)
    {
        std::string name;
        std::int32_t base_salary;
        int type;
        if (!(in >> type))
        {
            throw std::invalid_argument("No type was specified.");
        }
        if (type == 1)
        {
            Developer *developer = new Developer();
            in >> (*developer);
            employees_array._employees.emplace_back(developer);
        } else if (type == 2)
        {
            SalesManager *sales_manager = new SalesManager();
            in >> (*sales_manager);
            employees_array._employees.emplace_back(sales_manager);
        } else
        {
            throw std::invalid_argument("Unknown employee type.");
        }

        return in;
    }

    std::ifstream &operator>>(std::ifstream &in, EmployeesArray &employees_array)
    {
        std::string name;
        std::int32_t base_salary;
        std::int32_t n;
        if (!(in >> bin_manip::read_le_int32(n)))
        {
            throw std::invalid_argument("No number of employees was specified.");
        }
        for (size_t i = 0; i < n; ++i)
        {
            std::int32_t type;
            if (!(in >> bin_manip::read_le_int32(type)))
            {
                throw std::invalid_argument("No type was specified.");
            }
            if (type == 1)
            {
                Developer *developer = new Developer();
                in >> (*developer);
                employees_array._employees.emplace_back(developer);
            } else if (type == 2)
            {
                SalesManager *sales_manager = new SalesManager();
                in >> (*sales_manager);
                employees_array._employees.emplace_back(sales_manager);
            } else
            {
                throw std::invalid_argument("Unknown employee type.");
            }
        }

        return in;
    }
}