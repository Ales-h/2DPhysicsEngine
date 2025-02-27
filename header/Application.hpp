#ifndef PHYSOLVER_APPLICATION_HPP
#define PHYSOLVER_APPLICATION_HPP

#include <SDL.h>

#include "CollisionResolver.hpp"
#include "Object.hpp"
#include "RigidbodySystem.hpp"

namespace SceneManager {
struct Scene;
}

typedef int AppFlags;

enum AppFlags_ {
    AppFlags_None = 0,
    AppFlags_Gravity = 1 << 0,
    AppFlags_ShowCollisionPoints = 1 << 1,
    AppFlags_ShowVelocityVectors = 1 << 2,
    // to read if Simulation is running
    AppFlags_iSSimulationRunning = 1 << 5,
    // which tool is selected(spawn object, drag object, spiral)
    AppFlags_DragObjectsEvent = 1 << 29,
    AppFlags_SpiralEvent = 1 << 30,
    AppFlags_SpawnObjectsEvent = 1 << 31,

};

class Application {
   public:
    Application(int _fps = 30);
    ~Application();
    void addObject(Object* object);
    void removeObject(Object* object);

    void render();
    SDL_Texture* renderScenePreview(SceneManager::Scene* scene);

    void loadScene(SceneManager::Scene* scene);
    void clear();  // clears the scene to be able to load a new

    void run();

    AppFlags appFlags;
    Renderer* m_renderer;
    SDL_Window* m_window;
    RigidbodySystem* m_rbSystem;
    CollisionResolver* m_cResolver;
    std::vector<Object*> m_objects;
    SceneManager::Scene* m_scene;  // current scene

   private:
    int fps;
    Uint64 startTime;
};

#endif
