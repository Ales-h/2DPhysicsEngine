#ifndef PHYSOLVER_CIRCLE_SHAPE_HPP
#define PHYSOLVER_CIRCLE_SHAPE_HPP

#include "Shape.hpp"

class circleShape: public Shape {
public:
    double radius;
    Rigidbody* rigidbody;
    char type;

    circleShape(Rigidbody* rb, double _radius = 1);
    ~circleShape();
    void render(Renderer* renderer, int color) override;
    Vec2 center() const;
    Vec2 project(const Axis& axis) const;
    double momentOfInertia() override;
};

#endif
