#include "../header/Object.hpp"

#include <iostream>

#include "../header/Application.hpp"

Object::Object() { type = Object::Type::INVISIBLE; }

Object::~Object() {}

void Object::init(Application* app, Shape* _shape, Rigidbody* rb, Type t,
                  Color c) {
    type = t;
    color = c;

    if (type != Object::Type::FIXED) {
        app->rbSystem->addRigidbody(rb);

    }
    _shape->rigidbody = rb;
    shape = _shape;
    shape->rigidbody = rb;
    std::cout << shape->rigidbody->pos << "\n";

    app->addObject(this);
}

void Object::render(Renderer* renderer) {
    // std::cout << shape->rigidbody->getX() << "is accessed in Object\n";
    //std::cout << shape->rigidbody->pos << "\n";
    shape->render(renderer, color);
}

bool Object::isFixed() {
    if (type == FIXED) {
        return true;
    }
    return false;
}

void Object::translate(const Vec2& mtv) { shape->rigidbody->pos += mtv; }

double Object::inverseInertia(){
    if(!isFixed()){
        return 1/shape->momentOfInertia();
    } else {
        return 0;
    }
}

void Object::onClick() {}
