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
    ~Shape();
    virtual void render(Renderer* renderer, int color);
    virtual Vec2 project(Axis axis) const;
    virtual double momentOfInertia();
};

#endif
