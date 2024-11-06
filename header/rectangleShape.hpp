#ifndef PHYSOLVER_RECTANGLE_SHAPE_HPP
#define PHYSOLVER_RECTANGLE_SHAPE_HPP

#include "Shape.hpp"

class rectangleShape: public Shape {
public:
    int

    rectangleShape();
    ~rectangleShape();
    virtual void render(Renderer* renderer);
};

#endif
