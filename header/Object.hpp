#ifndef PHYSOLVER_OBJECT_HPP
#define PHYSOLVER_OBJECT_HPP

#include "Shape.hpp"

class Application;

class Object {
public:
    enum Type {
        FIXED,
        DYNAMIC,
        INVISIBLE
    };
    Shape* shape;
    Type type;
    Color color;
    int idx;

    Object();
    Object(const Object& ob);
    ~Object();
    void init(Application* app, Shape* _shape, Type t = DYNAMIC, Color c = BLUE);
    void render(Renderer* renderer);
    bool isFixed();
    void translate(const Vec2& mtv);
    double inverseInertia();
    virtual void onClick();
};

#endif
