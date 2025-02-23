#include "RigidBodySystem.hpp"

#include <algorithm>

#include "GravityGenerator.hpp"

RigidbodySystem::RigidbodySystem() {}

RigidbodySystem::~RigidbodySystem() {
    for(auto fg : forcegenerators){
        delete fg;
    }
}

void RigidbodySystem::addRigidbody(Rigidbody* rb) {
    rigidbodies.emplace_back(rb);
    rb->idx = rigidbodies.size() - 1;
}

void RigidbodySystem::removeRigidbody(Rigidbody* rb) {
    // swap with last element and resize
    rigidbodies[rb->idx] = rigidbodies.back();
    rigidbodies[rb->idx]->idx = rb->idx;
    rigidbodies.resize(rigidbodies.size() - 1);
}

void RigidbodySystem::addForceGenerator(ForceGenerator* fg) {
    forcegenerators.emplace_back(fg);
}

void RigidbodySystem::removeForceGenerator(ForceGenerator* fg) {
    // move the pointer to end of the vector
    auto it = std::remove(forcegenerators.begin(), forcegenerators.end(), fg);
    forcegenerators.erase(it, forcegenerators.end());
    delete fg;
}

bool RigidbodySystem::isGravityGenerator() {
    for (auto fg : forcegenerators) {
        if (dynamic_cast<GravityGenerator*>(fg)) {
            return true;
        }
    }
    return false;
}

GravityGenerator* RigidbodySystem::getGravityGenerator() {
    if (isGravityGenerator()) {
        for (auto fg : forcegenerators) {
            if (auto gg = dynamic_cast<GravityGenerator*>(fg)) {
                return gg;
            }
        }
    }
    return nullptr;
}

void RigidbodySystem::applyForces() {
    const int s = forcegenerators.size();

    for (int i = 0; i < s; ++i) {
        forcegenerators[i]->ApplyForce(rigidbodies);
    }
}

void RigidbodySystem::step(double dt) {
    applyForces();
    const int s = rigidbodies.size();

    for (int i = 0; i < s; ++i) {
        Rigidbody* rb = rigidbodies[i];
        Vec2 v0 = rb->v;
        Vec2 a0 = rb->f / rb->m;

        double omega0 = rb->omega;
        double theta0 = rb->theta;
        double epsilon0 = rb->epsilon;

        Vec2 vK1 = a0 * dt;
        Vec2 posK1 = v0 * dt;
        double omegaK1 = epsilon0 * dt;
        double thetaK1 = omega0 * dt;

        Vec2 vK2 = ((rb->f / rb->m) + vK1 * 0.5) * dt;
        Vec2 posK2 = (v0 + vK1 * 0.5) * dt;
        double omegaK2 = (epsilon0 + omegaK1 * 0.5) * dt;
        double thetaK2 = (omega0 + omegaK1 * 0.5) * dt;

        Vec2 vK3 = ((rb->f / rb->m) + vK2 * 0.5) * dt;
        Vec2 posK3 = (v0 + vK2 * 0.5) * dt;
        double omegaK3 = (epsilon0 + omegaK2 * 0.5) * dt;
        double thetaK3 = (omega0 + omegaK2 * 0.5) * dt;

        Vec2 vK4 = ((rb->f / rb->m) + vK3) * dt;
        Vec2 posK4 = (v0 + vK3) * dt;
        double omegaK4 = (epsilon0 + omegaK3) * dt;
        double thetaK4 = (omega0 + omegaK3) * dt;

        rb->v += (vK1 + vK2 * 2 + vK3 * 2 + vK4) / 6.0;
        rb->pos += (posK1 +  posK2 * 2 + posK3 * 2 + posK4) / 6.0;

        rb->omega += (omegaK1 + 2.0 * omegaK2 + 2.0 * omegaK3 + omegaK4) / 6.0;
        rb->theta += (thetaK1 + 2.0 * thetaK2 + 2.0 * thetaK3 + thetaK4) / 6.0;

        rb->f = {0, 0};
    }
}
