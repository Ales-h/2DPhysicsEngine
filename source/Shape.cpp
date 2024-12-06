#include "../header/Shape.hpp"
#include <stdexcept>

Shape::Shape(){}

Shape::~Shape(){}

void Shape::render(Renderer* renderer, int color){}
Vec2 Shape::project(Axis axis){
    throw std::runtime_error("trying to project a classic shape");
    return Vec2(0, 0);
}

double Shape::momentOfInertia(){
    throw std::runtime_error("calling momentOfInertia on Shape class");
    return 0;}


