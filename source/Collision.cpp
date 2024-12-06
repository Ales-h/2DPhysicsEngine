#include "../header/Collision.hpp"

Collision::Collision(Object* ObjectA, Object* ObjectB, Vec2 _mtv){
    mtv = _mtv;
    A = ObjectA;
    B = ObjectB;
    depth = mtv.magnitude();
    normal = mtv.normalize();
}

Collision::~Collision(){}
