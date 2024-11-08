#ifndef PHYSOLVER_CIRCLE_SHAPE_HPP
#define PHYSOLVER_CIRCLE_SHAPE_HPP

#include "Shape.hpp"

class circleShape: public Shape {
public:
    double radius;

    circleShape(double _radius = 1);
    ~circleShape();
    void render(Renderer* renderer);
    Vec2 center();
    Vec2 project(const Axis& axis);
private:
    Rigidbody* rigidbody;
};

#endif
