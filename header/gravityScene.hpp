#ifndef PHYSOLVER_GRAVITY_SCENE_HPP
#define PHYSOLVER_GRAVITY_SCENE_HPP

#include "Scene.hpp"

class gravityScene: public Scene {
public:
    gravityScene(Application* _app);

    void init() override;
    void clean() override;

private:
    Application* app;
};

#endif
