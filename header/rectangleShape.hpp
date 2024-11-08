#ifndef PHYSOLVER_RECTANGLE_SHAPE_HPP
#define PHYSOLVER_RECTANGLE_SHAPE_HPP

#include <vector>
#include "Shape.hpp"

class rectangleShape: public Shape {
public:
    double width;
    double height;

    rectangleShape(double _w = 1, double _h = 1);
    ~rectangleShape();
    void render(Renderer* renderer);
    Vec2 center();
    std::vector<Vec2> getVertices() const;
    Vec2 project(const Axis& axis);
private:
    Rigidbody* rigidbody;
};

#endif
