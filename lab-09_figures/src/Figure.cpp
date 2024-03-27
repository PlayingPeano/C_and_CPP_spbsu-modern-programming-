#include "Figure.hpp"
#include <iostream>

Figure::Figure(int id, int x, int y) : id(id), x(x), y(y) {}
Figure::~Figure() = default;

void Figure::move(int new_x, int new_y)
{
    x = new_x;
    y = new_y;
}

int Figure::getId() const
{
    return id;
}

void Figure::print() const
{
    std::cout << id << ": x = " << x << " y = " << y;
}