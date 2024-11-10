#include "../header/CollisionResolver.hpp"

#include <algorithm>
#include <limits>

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

Vec2 CollisionResolver::detectCollision(const circleShape* c2,
                                        const circleShape* c1) {
    Vec2 distanceVec = c1->center() - c2->center();
    if(distanceVec.magnitude() > c1->radius + c2->radius){
        return Vec2(0, 0);
    } else {
        Axis axis = Axis(distanceVec);
        double overlap = (c1->radius + c2->radius) - distanceVec.magnitude();
        return Vec2(axis.getX() * overlap, axis.getY() * overlap);
    }

}

void resolveCollision() {}

void checkCollisions(Application* app) {

}
