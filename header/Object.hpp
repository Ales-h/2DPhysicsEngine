#ifndef PHYSOLVER_OBJECT_HPP
#define PHYSOLVER_OBJECT_HPP

#include "Shape.hpp"

class Application;

class Object {
public:
    enum Color {
        GREEN,
        BLUE,
        PURPLE,
        YELLOW
    };
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
    ~Object();
    void init(Application* app, Shape* _shape, Rigidbody* rb, Type t = DYNAMIC, Color c = BLUE);
    void render(Renderer* renderer);
    bool isFixed();
    void translate(const Vec2& mtv);
    double inverseInertia();
    virtual void onClick();
};

#endif
