#include "../header/Object.hpp"

#include <iostream>

#include "../header/Application.hpp"

Object::Object() {
    shape = nullptr;
    color = Color::BLUE;
    type = Object::Type::INVISIBLE;
}

Object::~Object() {}

void Object::init(Application* app, Shape* _shape, Type t, Color c) {
    type = t;
    color = c;
    shape = _shape;

    if (type != Object::Type::FIXED) {
        app->m_rbSystem->addRigidbody(shape->rigidbody);
    } else {
        shape->rigidbody->m = INFINITY;
    }

    app->addObject(this);
}


void Object::render(Renderer* renderer) { shape->render(renderer, color); }

bool Object::isFixed() {
    if (type == FIXED) {
        return true;
    }
    return false;
}

void Object::translate(const Vec2& mtv) { shape->rigidbody->pos += mtv; }

double Object::inverseInertia() {
    if (!isFixed()) {
        return 1 / shape->momentOfInertia();
    } else {
        return 0;
    }
}

void Object::onClick() {}
