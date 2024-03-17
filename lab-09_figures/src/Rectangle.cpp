#include "Rectangle.hpp"
#include <iostream>
#include <cmath>

Rectangle::Rectangle(int id, int x, int y, int width, int height) : Figure(id, x, y), width(width), height(height) {}
Rectangle::~Rectangle() {}

void Rectangle::print() const 
{
    std::cout << "Rectangle " << id << ": x = " << x << " y = " << y << " width = " << width << " height = " << height << std::endl;
}

bool Rectangle::is_inside(int x, int y) const
{
    return abs(this->x - x) <= width && abs(this->y - y) <= height;
}

void Rectangle::zoom(int factor)
{
    x *= factor;
    y *= factor;
}
