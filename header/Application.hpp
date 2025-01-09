#ifndef PHYSOLVER_APPLICATION_HPP
#define PHYSOLVER_APPLICATION_HPP

#include <SDL.h>
#include "Object.hpp"
#include "RigidBodySystem.hpp"
#include "CollisionResolver.hpp"


class Application {
public:

    Application(int _fps = 60);
    ~Application();

    void render();
    void addObject(Object* object);
    void run();

    Renderer* m_renderer;
    SDL_Window* m_window;
    RigidbodySystem* m_rbSystem;
    CollisionResolver* m_cResolver;
    std::vector<Object*> m_objects;

private:
    int fps;
    Uint64 startTime;
};

#endif
