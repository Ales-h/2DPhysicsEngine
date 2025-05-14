#include "../header/SpiralGenerator.hpp"

#include "SDL3/SDL_mouse.h"

SpiralGenerator::SpiralGenerator() { magnitude = 10; }

SpiralGenerator::~SpiralGenerator() {}

void SpiralGenerator::ApplyForce(std::vector<Rigidbody*>& rbs) {
    float x, y = 0;
    SDL_GetMouseState(&x, &y);
    Vec2 mousePos = {((double)x)/100, -((double)y)/100};

    const int s = rbs.size();
    for (int i = 0; i < s; ++i) {
        Vec2 f = (mousePos - rbs[i]->pos); // Vector from rb to the mouse
        Vec2 dir = f.normalize();
        // more distance = less force
        rbs[i]->f += (dir * magnitude) / f.magnitude();

    }
}
