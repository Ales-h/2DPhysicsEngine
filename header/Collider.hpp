#ifndef PHYSOLVER_COLLIDER_HPP
#define PHYSOLVER_COLLIDER_HPP
#include "Shape.hpp"

class Collider {
public:
    Shape* shape;
    Collider();
    ~Collider();

    void virtual detectCollision();
};

#endif
