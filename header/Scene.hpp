#ifndef PHYSOLVER_SCENE_HPP
#define PHYSOLVER_SCENE_HPP

#include "Application.hpp"

class Scene {
public:

    Scene();
    ~Scene();
    virtual void init();
    virtual void clean();

private:
    Application* app;
};
#endif
