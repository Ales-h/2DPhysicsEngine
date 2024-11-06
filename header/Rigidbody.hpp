#ifndef PHYSOLVER_RIGIDBODY_HPP
#define PHYSOLVER_RIGIDBODY_HPP

#include "Vec2.hpp"

class Rigidbody {
   public:
    Vec2 pos;
    Vec2 v;    // velocity; rychlost
    Vec2 a;    // acceleration; zrychleni
    Vec2 f;    // force; sila
    double m;  // mass; hmotnost

    double theta;    // rotational angle
    double omega;    // angular velocity; uhlova rychlost
    double epsilon;  // angular acceleration; uhlove zrychleni

    Rigidbody(double p_x = 0, double p_y = 0, double v_x = 0, double v_y = 0,
              double a_x = 0, double a_y = 0, double f_x = 0, double f_y = 0,
              double _m = 1, double _theta = 0, double _omega = 0,
              double _epsilon = 0);
    ~Rigidbody();

    double getX() const;
    double getY() const;
};

#endif
