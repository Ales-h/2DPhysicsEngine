#ifndef PHYSOLVER_APPLICATION_HPP
#define PHYSOLVER_APPLICATION_HPP

#include <SDL.h>

#include "CollisionResolver.hpp"
#include "Object.hpp"
#include "RigidBodySystem.hpp"

namespace SceneManager {
struct Scene;
}

typedef int AppFlags;

enum AppFlags_ {
    AppFlags_None = 0,
    AppFlags_Gravity = 1 << 0,
    AppFlags_ShowCollisionPoints = 1 << 1,
    AppFlags_ShowVelocityVectors = 1 << 2,
};

class Application {
   public:
    Application(int _fps = 60);
    ~Application();

    void render();
    void loadScene(SceneManager::Scene* scene);
    SDL_Texture* renderScenePreview(SceneManager::Scene* scene);
    void addObject(Object* object);
    void run();

    AppFlags appFlags;
    Renderer* m_renderer;
    SDL_Window* m_window;
    RigidbodySystem* m_rbSystem;
    CollisionResolver* m_cResolver;
    std::vector<Object*> m_objects;
    std::string sceneName;

   private:
    int fps;
    Uint64 startTime;
};

#endif
