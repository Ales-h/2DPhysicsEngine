#ifndef PHYSOLVER_OBJECT_HPP
#define PHYSOLVER_OBJECT_HPP

#include "Shape.hpp"
#include "RigidbodySystem.hpp"
#include "Application.hpp"

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

    Object();
    ~Object();
    void init(Application* app, Shape* _shape, Rigidbody* rb, Type t = DYNAMIC, Color c = BLUE);
    void render(Renderer* renderer);
    virtual void onClick();
};

#endif
