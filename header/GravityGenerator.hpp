#ifndef PHYSOLVER_GRAVITYGENERATOR_HPP
#define PHYSOLVER_GRAVITYGENERATOR_HPP

#include "ForceGenerator.hpp"

class GravityGenerator : public ForceGenerator {
   public:
    GravityGenerator();
    ~GravityGenerator();

    void ApplyForce(std::vector<Rigidbody*>& rbs);
    double _g;
};

#endif
