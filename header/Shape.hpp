#ifndef PHYSOLVER_SHAPE_HPP
#define PHYSOLVER_SHAPE_HPP
#include "Rigidbody.hpp"
#include "Axis.hpp"
#include "Renderer.hpp"

class Shape{
public:
    Rigidbody* rigidbody;
    char type; // r = rectangle, c = circle

    Shape(Rigidbody* rb, char _type);
    Shape(const Shape& sh);
    virtual ~Shape();
    virtual void render(Renderer* renderer, int color) = 0;
    virtual Vec2 project(const Axis& axis) const = 0;
    virtual double momentOfInertia() = 0;
};

#endif
