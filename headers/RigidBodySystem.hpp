#ifndef PHYSOLVER_RIGIDBODYSYSTEM_HPP
#define PHYSOLVER_RIGIDBODYSYSTEM_HPP

#include "Rigidbody.hpp"
#include "ForceGenerator.hpp"
#include <vector>

class RigidbodySystem{
public:
    RigidbodySystem();
    ~RigidbodySystem();

    void addRigidbody(Rigidbody* rb);
    void removeRigidbody(Rigidbody* rb);

    void addForceGenerator(ForceGenerator* fg);

    void removeForceGenerator(ForceGenerator* fg);

    void applyForces();

    void step(int step);

private:
    std::vector<Rigidbody*> _rigidbodies;
    std::vector<ForceGenerator*> _forcegenerators;


};
#endif
