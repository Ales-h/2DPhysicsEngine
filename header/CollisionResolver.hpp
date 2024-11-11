#ifndef PHYSOLVER_COLLISION_RESOLVER_HPP
#define PHYSOLVER_COLLISION_RESOLVER_HPP

#include "circleShape.hpp"
#include "rectangleShape.hpp"
#include "Object.hpp"

class Application;

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
    Vec2 detectCollision(const Shape* shape1, const Shape* shape2);

    void resolveCollision();

    void resolveMTV(Object* object1, Object* object2, Vec2 mtv);

    void checkCollisions(Application* app);

};

#endif
