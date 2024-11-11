#ifndef PHYSOLVER_RECTANGLE_SHAPE_HPP
#define PHYSOLVER_RECTANGLE_SHAPE_HPP

#include <vector>
#include "Shape.hpp"

class rectangleShape: public Shape {
public:
    double width;
    double height;
    Rigidbody* rigidbody;

    rectangleShape(double _w = 1, double _h = 1);
    ~rectangleShape();
    void render(Renderer* renderer, int color);
    Vec2 center();
    std::vector<Vec2> getVertices() const;
    Vec2 closestVertex(const Vec2 p) const;
    std::vector<Axis> getAxes() const;
    Vec2 project(const Axis& axis) const;
};

#endif
