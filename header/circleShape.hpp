#ifndef PHYSOLVER_CIRCLE_SHAPE_HPP
#define PHYSOLVER_CIRCLE_SHAPE_HPP

#include "Shape.hpp"

class circleShape: public Shape {
public:
    double radius;

    circleShape(Rigidbody* rb, double _radius = 1);
    circleShape(const circleShape& circ): Shape(circ), radius(circ.radius){}
    ~circleShape() override = default;
    void render(Renderer* renderer, int color) override;
    void renderOutline(Renderer* renderer, int color) override;
    Vec2 center() const;
    Vec2 project(const Axis& axis) const override;
    double momentOfInertia() override;
    bool containsPoint(const Vec2& point) override;
    double getRadius() override;
};

#endif
