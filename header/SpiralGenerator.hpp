#ifndef PHYSOLVER_SPIRALGENERATOR_HPP
#define PHYSOLVER_SPIRALGENERATOR_HPP

#include "ForceGenerator.hpp"

class SpiralGenerator: public ForceGenerator {
   public:
    SpiralGenerator();
    ~SpiralGenerator();

    void ApplyForce(std::vector<Rigidbody*>& rbs);
    double magnitude;
};

#endif

