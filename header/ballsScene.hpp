#ifndef PHYSOLVER_BALLS_SCENE_HPP
#define PHYSOLVER_BALLS_SCENE_HPP

#include "Scene.hpp"

class ballsScene: public Scenea {
public:
    ballsScene(Application* _app);

    void init() override;
    void clean() override;

private:
    Application* app;
};

#endif
