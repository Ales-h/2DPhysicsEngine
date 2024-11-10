#ifndef PHYSOLVER_COLLISION_RESOLVER_HPP
#define PHYSOLVER_COLLISION_RESOLVER_HPP

#include "circleShape.hpp"
#include "rectangleShape.hpp"
#include "Application.hpp"

class CollisionResolver {
public:
    CollisionResolver();
    ~CollisionResolver();

    bool overlap(const Vec2 v1, const Vec2 v2) const;

    double getOverlap(const Vec2 v1, const Vec2 v2) const;

    Vec2 detectCollision(const circleShape* circle, const rectangleShape* rect);
    Vec2 detectCollision(const rectangleShape* rect, const circleShape* circle);
    Vec2 detectCollision(const rectangleShape* r1, const rectangleShape* r2);
    Vec2 detectCollision(const circleShape* c2, const circleShape* c1);

    void resolveCollision();

    void checkCollisions(Application* app);

};

#endif
