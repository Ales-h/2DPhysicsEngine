#ifndef PHYSOLVER_RIGIDBODYSYSTEM_HPP
#define PHYSOLVER_RIGIDBODYSYSTEM_HPP

#include "Rigidbody.hpp"
#include "ForceGenerator.hpp"
#include "GravityGenerator.hpp"
#include <vector>

class RigidbodySystem{
public:
    RigidbodySystem();
    ~RigidbodySystem();


    void addRigidbody(Rigidbody* rb);
    void removeRigidbody(Rigidbody* rb);

    void addForceGenerator(ForceGenerator* fg);
    void removeForceGenerator(ForceGenerator* fg);
    bool isGravityGenerator();
    GravityGenerator* getGravityGenerator();


    void applyForces();

    void step(double dt);

private:
    std::vector<Rigidbody*> rigidbodies;
    std::vector<ForceGenerator*> forcegenerators;


};
#endif
