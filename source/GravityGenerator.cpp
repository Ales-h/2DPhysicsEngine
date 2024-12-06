#include "../header/GravityGenerator.hpp"
#include <iostream>

GravityGenerator::GravityGenerator(){
    _g = -9.8;
}

GravityGenerator::~GravityGenerator(){}

void GravityGenerator::ApplyForce(std::vector<Rigidbody*>& rbs){
     const int s = rbs.size();

    for (int i = 0; i < s; ++i){

        rbs[i]->f.y += _g * rbs[i]->m;
    }
}
