#ifndef PHYSOLVER_RIGIDBODY_HPP
#define PHYSOLVER_RIGIDBODY_HPP

#include "Vec2.hpp"

class Rigidbody {
public:
    Vec2 pos;
    Vec2 v; // velocity; rychlost
    Vec2 a; // acceleration; zrychleni
    double m; // mass; hmotnost

    double theta; // rotational angle
    double ang_v; // angular velocity; uhlova rychlost
private:

};

#endif
