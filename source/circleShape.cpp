#include "../header/circleShape.hpp"

circleShape::circleShape(Rigidbody* rb, double _radius) : Shape(rb, 'c') {
    radius = _radius;
}

void circleShape::render(Renderer* renderer, int color) {
    renderer->drawCircle(rigidbody->pos, radius, color);
}

void circleShape::renderOutline(Renderer* renderer, int color) {
    renderer->drawCircleOutline(rigidbody->pos, radius, color);
}

Vec2 circleShape::center() const { return rigidbody->pos; }
// project the shape onto an axis and returns an interval
Vec2 circleShape::project(const Axis& axis) const {
    const double proj =
        (rigidbody->getX() * axis.getX() + rigidbody->getY() * axis.getY());

    return Vec2(proj - radius, proj + radius);
}
double circleShape::momentOfInertia() { return 1. / 2. * rigidbody->m * radius * radius; }

bool circleShape::containsPoint(const Vec2& point) {
    Vec2 edge = point - rigidbody->pos;
    double distanceSq = (edge.x * edge.x) + (edge.y * edge.y);
    if (distanceSq < (radius * radius)) {
        return true;
    }
    return false;
}
