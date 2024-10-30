#ifndef PHYSOLVER_GRAVITYGENERATOR_HPP
#define PHYSOLVER_GRAVITYGENERATOR_HPP

#include "ForceGenerator.hpp"

class GravityGenerator: public ForceGenerator {
public:
    GravityGenerator();
    ~GravityGenerator();

    void ApplyForce(std::vector<Rigidbody*>& rbs);
private:
    double _g;

};

#endif
