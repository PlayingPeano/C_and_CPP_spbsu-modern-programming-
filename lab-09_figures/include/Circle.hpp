#pragma once

#include "Figure.hpp"

class Circle : public Figure {
public:
    Circle(int id, int x, int y, int radius, const char* label);
    virtual ~Circle() override;

    virtual void print() const override;
    virtual bool is_inside(int x, int y) const override;
    virtual void zoom(int factor) override;

private:
    int radius;
    const char* label;
};
