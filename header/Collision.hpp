#ifndef PHYSOLVER_COLLIDER_HPP
#define PHYSOLVER_COLLIDER_HPP
#include "Vec2.hpp"
#include "Object.hpp"
#include <vector>

struct Collision{
    Vec2 mtv;
    Object* A;
    Object* B;
    double depth;
    Vec2 normal;
    std::vector<Vec2> contacts;
    int contactCounts;
    Collision(Object* ObjectA, Object* ObjectB, Vec2 _mtv);
    ~Collision();
};

#endif
