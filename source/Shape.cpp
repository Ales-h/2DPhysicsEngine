#include "../header/Shape.hpp"
#include <stdexcept>
#include "RigidBodySystem.hpp"
#include <iostream>

Shape::Shape(Rigidbody* rb, char _type): rigidbody(rb), type(_type){}

Shape::Shape(const Shape& sh){
    rigidbody = new Rigidbody(*sh.rigidbody);
    type = sh.type;
}

Shape::~Shape(){
    delete rigidbody;
}

void Shape::render(Renderer* renderer, int color){}
Vec2 Shape::project(const Axis& axis) const{
    throw std::runtime_error("trying to project a classic shape");
    return Vec2(0, 0);
}

double Shape::momentOfInertia(){
    throw std::runtime_error("calling momentOfInertia on Shape class");
    return 0;}


