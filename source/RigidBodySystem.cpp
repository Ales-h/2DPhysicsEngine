#include "RigidBodySystem.hpp"
#include "GravityGenerator.hpp"

RigidbodySystem::RigidbodySystem(){
}


RigidbodySystem::~RigidbodySystem(){
}

void RigidbodySystem::addRigidbody(Rigidbody* rb){
    rigidbodies.emplace_back(rb);
    rb->idx = rigidbodies.size()-1;
}

void RigidbodySystem::removeRigidbody(Rigidbody* rb){
    // swap with last element and resize
    rigidbodies[rb->idx] = rigidbodies.back();
    rigidbodies[rb->idx]->idx = rb->idx;
    rigidbodies.resize(rigidbodies.size() - 1);
}

void RigidbodySystem::addForceGenerator(ForceGenerator* fg){
    forcegenerators.emplace_back(fg);
}
// TODO
void RigidbodySystem::removeForceGenerator(ForceGenerator* fg){
}

bool RigidbodySystem::isGravityGenerator(){
    for (auto fg : forcegenerators){
        if(dynamic_cast<GravityGenerator*>(fg)){
            return true;
        }
    }
    return false;
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
        //std::cout << "position: " << rb->getX() << " " << rb->getY() << "\n";
        //std::cout << "velocity: " << rb->v.x << " " << rb->v.y << "\n";

        rb->a = rb->f / rb->m;
        rb->v += rb->a * dt;
        rb->pos += rb->v * dt;

        rb->omega += rb->epsilon * dt;
        rb->theta += rb->omega * dt;

        rb->f = {0, 0};
    }
}
