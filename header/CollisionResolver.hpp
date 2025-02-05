#ifndef PHYSOLVER_COLLISION_RESOLVER_HPP
#define PHYSOLVER_COLLISION_RESOLVER_HPP

#include <tuple>
#include <utility>
#include <vector>

#include "Collision.hpp"
#include "Object.hpp"
#include "circleShape.hpp"
#include "rectangleShape.hpp"

class Application;

class CollisionResolver {
   public:
    CollisionResolver();
    ~CollisionResolver();
    void checkCollisions(Application* app);
    void renderCollisionPoints(Renderer* renderer);

   private:
    std::vector<Collision> broadPhaseCheck(std::vector<Object*> objects);
    // radius is for rectangles the longer side of the rect
    bool broadPhaseDetection(const Vec2 posA, const Vec2 posB, const double radiusA,
                         const double radiusB);
    Vec2 detectCollision(const circleShape* circle, const rectangleShape* rect);
    Vec2 detectCollision(const rectangleShape* rect, const circleShape* circle);
    Vec2 detectCollision(const rectangleShape* r1, const rectangleShape* r2);
    Vec2 detectCollision(const circleShape* c2, const circleShape* c1);
    Vec2 detectCollision(const Shape* shape1, const Shape* shape2);

    std::pair<double, Vec2> pointSegmentDistance(const Vec2& p, const Vec2& a,
                                                 const Vec2& b);

    void getCollisionPoints(Collision* collision);

    std::tuple<int, Vec2, Vec2> getCollisionPoint(const Vec2& centerA,
                                                  const double& radiusA,
                                                  const Vec2& centerB,
                                                  const double& radiusB);

    std::tuple<int, Vec2, Vec2> getCollisionPoint(const Vec2& centerA,
                                                  const double& radiusA,
                                                  const std::vector<Vec2>& verticesB);

    std::tuple<int, Vec2, Vec2> getCollisionPoint(const std::vector<Vec2>& verticesA,
                                                  const std::vector<Vec2>& verticesB);

    void resolveCollision(Collision* collision);
    void applyFriction(Collision* collision, std::array<double, 2>& jArray);
    void resolveMTV(Collision* collision);

   public:
    // needs to be floats for ImGui SlideFloat
    float _e;  // součinitel restituce
    float staticFriction;
    float dynamicFriction;

   private:
    std::vector<Collision> cpsToRender;
};

#endif
