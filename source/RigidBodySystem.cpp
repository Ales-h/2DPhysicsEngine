#include "../headers/RigidBodySystem.hpp"

RigidbodySystem::RigidbodySystem(){
}


RigidbodySystem::~RigidbodySystem(){
}

void RigidbodySystem::addRigidbody(Rigidbody* rb){
}

void RigidbodySystem::removeRigidbody(Rigidbody* rb){
}

void RigidbodySystem::addForceGenerator(ForceGenerator* fg){
}

void RigidbodySystem::removeForceGenerator(ForceGenerator* fg){
}

void RigidbodySystem::applyForces(){
    const int s =  forcegenerators.size();

    for(int i = 0; i < s; ++i){
        forcegenerators[i]->ApplyForce(rigidbodies);
    }
}

void RigidbodySystem::step(double dt){

    applyForces();
    const int s = rigidbodies.size();

    for(int i = 0; i < s; ++i){
        Rigidbody* rb = rigidbodies[i];

        rb->a += rb->f / rb->m;
        rb->v += rb->a * dt;
        rb->pos += rb->v * dt;

        rb->f = {0, 0};
    }
}
