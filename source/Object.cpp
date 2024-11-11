#include "../header/Object.hpp"
#include "../header/Application.hpp"

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

void Object::render(Renderer* renderer){
    shape->render(renderer, color);
}

bool Object::isFixed(){
    if(type == FIXED){
        return true;
    }
    return false;
}

void Object::translate(const Vec2& mtv){
    shape->rigidbody->pos += mtv;
}
