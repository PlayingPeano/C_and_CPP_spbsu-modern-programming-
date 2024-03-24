#pragma once

#include "employees.h"
#include "bin_manip.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

namespace Employee
{
    class Employee
    {
    protected:
        std::string _name;
        std::int32_t _base_salary;
    public:

        Employee(std::string name = "", std::int32_t base_salary = 0);

        virtual ~Employee();

        virtual std::int32_t salary() const = 0;

        virtual void read_text(std::istream &in) = 0;

        virtual void read_bin(std::ifstream &in) = 0;

        virtual void write_text(std::ostream &out) const = 0;

        virtual void write_bin(std::ofstream &out) const = 0;

        friend std::ostream &operator<<(std::ostream &out, const Employee &employee);

        friend std::istream &operator>>(std::istream &in, Employee &employee);

        friend std::ofstream &operator<<(std::ofstream &, const Employee &);

        friend std::ifstream &operator>>(std::ifstream &, Employee &);
    };

    class Developer : public Employee
    {
    public:
        Developer(std::string name = "", std::int32_t base_salary = 0, bool has_bonus = 0);

        std::int32_t salary() const override;

        void read_text(std::istream &in) override;

        void read_bin(std::ifstream &in) override;

        void write_text(std::ostream &out) const override;

        void write_bin(std::ofstream &out) const override;

    private:
        bool _has_bonus;
    };

    class SalesManager : public Employee
    {
    public:
        SalesManager(std::string name = "", std::int32_t base_salary = 0, std::int32_t sold_nm = 0, std::int32_t price = 0);

        std::int32_t salary() const override;

        void read_text(std::istream &in) override;

        void read_bin(std::ifstream &in) override;

        void write_text(std::ostream &out) const override;

        void write_bin(std::ofstream &out) const override;

    private:
        std::int32_t _sold_nm, _price;
    };

    class EmployeesArray
    {
    public:
        EmployeesArray(const EmployeesArray &employeesArray) = delete;

        EmployeesArray();

        ~EmployeesArray();

        std::int32_t total_salary() const;

        void save(std::string str) const;

        void load(std::string str);

        friend std::ostream &operator<<(std::ostream &out, const EmployeesArray &employees_array);
        friend std::ofstream &operator<<(std::ofstream &out, const EmployeesArray &employees_array);
        friend std::istream &operator>>(std::istream &in, EmployeesArray &employees_array);
        friend std::ifstream &operator>>(std::ifstream &in, EmployeesArray &employees_array);

    private:
        std::vector<Employee*> _employees;
    };
}