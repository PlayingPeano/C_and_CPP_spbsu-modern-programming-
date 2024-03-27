#include "Figure.hpp"

class Rectangle : public Figure {
public:
    Rectangle(int id, int x, int y, int width, int height);
    virtual ~Rectangle() override;

    virtual void print() const override;
    virtual bool is_inside(int x, int y) const override;
    virtual void zoom(int factor) override;

private:
    int width;
    int height;
};
