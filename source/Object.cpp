#include "../header/Object.hpp"

Object::Object(){
    type = Object::Type::INVISIBLE;
}

Object::~Object(){}

void Object::init(Application* app, Shape* _shape, Rigidbody* rb, Type t, Color c){
    type = t;
    color = c;

    if(type != Object::Type::FIXED){
        app->rbSystem->addRigidbody(rb);
    }
    shape = _shape;
    shape->rigidbody = rb;

    app->addObject(this);
}
