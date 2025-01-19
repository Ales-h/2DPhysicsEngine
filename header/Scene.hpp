#ifndef PHYSOLVER_SCENE_HPP
#define PHYSOLVER_SCENE_HPP

#include "Application.hpp"

class Scenea {
public:

    Scenea();
    ~Scenea();
    virtual void init();
    virtual void clean();

private:
    Application* app;
};
#endif
