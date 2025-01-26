#ifndef PHYSOLVER_EVENTS_HPP
#define PHYSOLVER_EVENTS_HPP
#include "Application.hpp"
#include "SpiralGenerator.hpp"

namespace Events {

void handleEvents(Application* app, bool &isRunning, int& selected);

void spawnEvent(Application* app);
void dragEvent();
int clickedOnObject(Application* app);

}

#endif
