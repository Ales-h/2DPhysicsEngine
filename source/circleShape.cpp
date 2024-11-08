#include "../header/circleShape.hpp"

circleShape::circleShape(double _radius) {
    radius = _radius;
}

circleShape::~circleShape() {
}

void circleShape::render(Renderer* renderer){
}

Vec2 circleShape::center(){
    return rigidbody->pos;
}
//project the shape onto an axis and returns an interval
Vec2 circleShape::project(const Axis& axis){
    const double proj = (rigidbody->getX() * axis.getX() + rigidbody->getY() * axis.getY());

    return Vec2(proj - radius, proj + radius);
}
