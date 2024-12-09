#include "../header/circleShape.hpp"
#include <iostream>

circleShape::circleShape(double _radius) {
    radius = _radius;
}

circleShape::~circleShape() {
}

void circleShape::render(Renderer* renderer, int color){
    renderer->drawCircle(rigidbody->pos, radius, color);
}

Vec2 circleShape::center() const{
    return rigidbody->pos;
}
//project the shape onto an axis and returns an interval
Vec2 circleShape::project(const Axis& axis) const{
    const double proj = (rigidbody->getX() * axis.getX() + rigidbody->getY() * axis.getY());

    return Vec2(proj - radius, proj + radius);
}
// TODO
double circleShape::momentOfInertia(){
    return 1./2. * rigidbody->m * radius * radius;
}
