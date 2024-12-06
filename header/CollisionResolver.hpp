#ifndef PHYSOLVER_COLLISION_RESOLVER_HPP
#define PHYSOLVER_COLLISION_RESOLVER_HPP

#include <tuple>
#include <utility>
#include "circleShape.hpp"
#include "rectangleShape.hpp"
#include "Object.hpp"
#include "Collision.hpp"

class Application;

class CollisionResolver {
public:
    CollisionResolver();
    ~CollisionResolver();
    void checkCollisions(Application* app);
    void renderCollisionPoints(Renderer* renderer);

private:

    Vec2 detectCollision(const circleShape* circle, const rectangleShape* rect);
    Vec2 detectCollision(const rectangleShape* rect, const circleShape* circle);
    Vec2 detectCollision(const rectangleShape* r1, const rectangleShape* r2);
    Vec2 detectCollision(const circleShape* c2, const circleShape* c1);
    Vec2 detectCollision(const Shape* shape1, const Shape* shape2);

    std::pair<double, Vec2> pointSegmentDistance(Vec2& p, Vec2& a, Vec2& b);

    void getCollisionPoints(Collision* collision);
    std::tuple<int, Vec2, Vec2> getCollisionPoint(const std::vector<Vec2>& verticesA, const std::vector<Vec2>& verticesB);

    void resolveCollision(Collision* collision);

    void resolveMTV(Collision* collision);


    double _e; // součinitel restituce
    std::vector<Collision> collisions;
    std::vector<Collision> cpsToRender;
};

#endif
