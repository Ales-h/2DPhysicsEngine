#ifndef PHYSOLVER_FORCEGENERATOR_HPP
#define PHYSOLVER_FORCEGENERATOR_HPP

#include <vector>
#include "Rigidbody.hpp"

class ForceGenerator{
public:
    virtual ~ForceGenerator();

    void virtual ApplyForce(std::vector<Rigidbody>& rbs);
};

#endif
