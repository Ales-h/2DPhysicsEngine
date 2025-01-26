#include "../header/Shape.hpp"
#include <stdexcept>

Shape::Shape(Rigidbody* rb, char _type): rigidbody(rb), type(_type){}

Shape::Shape(const Shape& sh){
    rigidbody = new Rigidbody(*sh.rigidbody);
    type = sh.type;
}

Shape::~Shape(){
    delete rigidbody;
}

