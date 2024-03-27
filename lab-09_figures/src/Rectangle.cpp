#include "Rectangle.hpp"
#include <iostream>
#include <cmath>

Rectangle::Rectangle(int id, int x, int y, int width, int height) : Figure(id, x, y), width(width), height(height) {}
Rectangle::~Rectangle() = default;

void Rectangle::print() const 
{
    std::cout << "Rectangle ";
    Figure::print();
    std::cout << " width = " << width << " height = " << height << std::endl;
}

bool Rectangle::is_inside(int x, int y) const
{
    return 2 * abs(this->x - x) <= width && 2 * abs(this->y - y) <= height;
}

void Rectangle::zoom(int factor)
{
    height *= factor;
    width *= factor;
}
