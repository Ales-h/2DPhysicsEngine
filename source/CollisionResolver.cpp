#include "../header/CollisionResolver.hpp"
#include "../header/Application.hpp"


#include <algorithm>
#include <limits>
#include <stdexcept>

CollisionResolver::CollisionResolver() {}

CollisionResolver::~CollisionResolver() {}

bool CollisionResolver::overlap(const Vec2 v1, const Vec2 v2) const {
    if (v1.x < v2.y && v2.x < v1.y) {
        return true;
    }
    return false;
}

double CollisionResolver::getOverlap(const Vec2 v1, const Vec2 v2) const {
    double overlap_start = std::max(v1.x, v2.x);
    double overlap_end = std::min(v1.y, v2.y);

    return std::max(0.0, overlap_end - overlap_start);
}

Vec2 CollisionResolver::detectCollision(const rectangleShape* rect,
                                        const circleShape* circle) {

    double minOverlap = std::numeric_limits<double>::max();
    Axis mtvAxis;
    Vec2 center = circle->center();

    std::vector<Axis> axes = rect->getAxes();

    for (int i = 0; i < axes.size(); ++i) {
        const Axis axis = axes[i];

        Vec2 proj1 = rect->project(axis);
        Vec2 proj2 = circle->project(axis);
        if (!overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }
    return Vec2(mtvAxis.getX() * minOverlap, mtvAxis.getY() * minOverlap);
}

Vec2 CollisionResolver::detectCollision(const circleShape* circle,
                                        const rectangleShape* rect) {
    return detectCollision(rect, circle);
}

Vec2 CollisionResolver::detectCollision(const rectangleShape* r1,
                                        const rectangleShape* r2) {
    std::vector<Axis> axes1 = r1->getAxes();
    std::vector<Axis> axes2 = r2->getAxes();
    double minOverlap = std::numeric_limits<double>::max();
    Axis mtvAxis;

    for (int i = 0; i < axes1.size(); ++i) {
        const Axis axis = axes1[i];

        Vec2 proj1 = r1->project(axis);
        Vec2 proj2 = r2->project(axis);
        if (!overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }

    for (int i = 0; i < axes2.size(); ++i) {
        const Axis axis = axes2[i];

        Vec2 proj1 = r1->project(axis);
        Vec2 proj2 = r2->project(axis);
        if (!overlap(proj1, proj2)) {
            return Vec2(0, 0);
        } else {
            double ol = getOverlap(proj1, proj2);
            if (ol < minOverlap) {
                minOverlap = ol;
                mtvAxis = axis;
            }
        }
    }
    return Vec2(mtvAxis.getX() * minOverlap, mtvAxis.getY() * minOverlap);

}

Vec2 CollisionResolver::detectCollision(const circleShape* c1,
                                        const circleShape* c2) {
    Vec2 distanceVec = c1->center() - c2->center();
    if(distanceVec.magnitude() > c1->radius + c2->radius){
        return Vec2(0, 0);
    } else {
        Axis axis = Axis(distanceVec);
        double overlap = (c1->radius + c2->radius) - distanceVec.magnitude();
        return Vec2(axis.getX() * overlap, axis.getY() * overlap);
    }

}

// Quick fix - refactor later into a shape/collider function
Vec2 CollisionResolver::detectCollision(const Shape* shape1, const Shape* shape2) {
    if (auto circle = dynamic_cast<const circleShape*>(shape1)) {
        if (auto rect = dynamic_cast<const rectangleShape*>(shape2)) {
            return detectCollision(circle, rect);
        }
    }
    else if (auto rect = dynamic_cast<const rectangleShape*>(shape1)) {
        if (auto circle = dynamic_cast<const circleShape*>(shape2)) {
            return detectCollision(rect, circle);
        }
    }
    else if (auto c1= dynamic_cast<const circleShape*>(shape1)) {
        if (auto c2 = dynamic_cast<const circleShape*>(shape2)) {
            return detectCollision(c1, c2);
        }
    }
    else if (auto r1= dynamic_cast<const rectangleShape*>(shape1)) {
        if (auto r2 = dynamic_cast<const rectangleShape*>(shape2)) {
            return detectCollision(r1, r2);
        }
    }
    throw std::runtime_error("Function for collision detection not found");
    return Vec2(0, 0);
}

void CollisionResolver::resolveCollision() {
// TODO
}

// TODO Refactor
void CollisionResolver::resolveMTV(Object* object1, Object* object2, Vec2 mtv) {
    bool isFixed1 = object1->isFixed();
    bool isFixed2 = object2->isFixed();

    Vec2 direction = object2->shape->rigidbody->pos - object1->shape->rigidbody->pos;

    if ((mtv.dot(direction)) < 0) {
        mtv = -mtv;
    }

    if (isFixed1 && !isFixed2) {
        object2->translate(mtv);
    } else if (!isFixed1 && isFixed2) {
        object1->translate(-mtv);
    } else if (!isFixed1 && !isFixed2) {
        double mass1 = object1->shape->rigidbody->m;
        double mass2 = object2->shape->rigidbody->m;
        double totalMass = mass1 + mass2;

        Vec2 mtv1 = mtv * (mass2 / totalMass);
        Vec2 mtv2 = mtv * (mass1 / totalMass);

        object1->translate(-mtv1);
        object2->translate(mtv2);
    }
}

void CollisionResolver::checkCollisions(Application* app) {
    for(int i = 0; i < app->objects.size(); ++i){
        Object* object1 = app->objects[i];
        for(int j = i+1; j < app->objects.size(); ++j){
            Object* object2 = app->objects[j];
            Vec2 mtv = detectCollision(object1->shape, object2->shape);
            if(mtv.x == 0 && mtv.y == 0) {
                continue;
            }
            resolveMTV(object1, object2, mtv);
        }
    }
}
