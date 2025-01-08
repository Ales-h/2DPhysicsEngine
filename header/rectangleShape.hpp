#ifndef PHYSOLVER_RECTANGLE_SHAPE_HPP
#define PHYSOLVER_RECTANGLE_SHAPE_HPP

#include <vector>
#include "Rigidbody.hpp"
#include "Shape.hpp"

class rectangleShape: public Shape {
public:
    double width;
    double height;
    Rigidbody* rigidbody;
    int id;

    rectangleShape(Rigidbody* rb, double _w = 1, double _h = 1, int _id=0);
    ~rectangleShape();
    void render(Renderer* renderer, int color) override;
    Vec2 center();
    std::array<Vec2, 4> getVertices() const;
    Vec2 closestVertex(const Vec2 p) const;
    std::vector<Axis> getAxes() const;
    Vec2 project(const Axis& axis) const;
    double momentOfInertia() override;
};

#endif
