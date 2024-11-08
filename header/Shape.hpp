#ifndef PHYSOLVER_SHAPE_HPP
#define PHYSOLVER_SHAPE_HPP
#include "Rigidbody.hpp"
#include "Axis.hpp"

class Shape{
public:

    Shape();
    ~Shape();
    virtual void render(Renderer* renderer);
    virtual Vec2 project(Axis axis);
private:
    Rigidbody* rigidbody;
};

#endif
