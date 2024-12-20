#ifndef PHYSOLVER_APPLICATION_HPP
#define PHYSOLVER_APPLICATION_HPP

#include <SDL.h>
#include "Object.hpp"
#include "RigidBodySystem.hpp"
#include "CollisionResolver.hpp"


class Application {
public:
    Renderer* renderer;
    RigidbodySystem* rbSystem;
    CollisionResolver* cResolver;
    std::vector<Object*> objects;

    Application(SDL_Renderer* _renderer, int _fps);
    ~Application();

    void init();

    void render();


    void addObject(Object* object);

    void run();
private:
    int fps;
    Uint64 startTime;
};

#endif
