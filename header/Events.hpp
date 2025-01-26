#ifndef PHYSOLVER_EVENTS_HPP
#define PHYSOLVER_EVENTS_HPP
#include "Application.hpp"
#include "SpiralGenerator.hpp"

namespace Events {

void handleEvents(Application* app, bool &isRunning, Object* selected);

void spawnEvent(Application* app);
void dragEvent();
Object* clickedOnObject(Application* app);

}

#endif
