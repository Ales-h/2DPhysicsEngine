#include "Events.hpp"

#include <SDL_timer.h>

#include <iostream>

#include "Application.hpp"
#include "backends/imgui_impl_sdl2.h"

namespace Events {

void handleEvents(Application* app, bool& isRunning, int& selected) {
    SDL_Event event;
    static bool isMouseHeld = false;
    static SpiralGenerator* spiralGen = nullptr;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                std::exit(0);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (!isMouseHeld && !(app->appFlags & AppFlags_iSSimulationRunning)) {
                        selected = clickedOnObject(app);
                    }
                    isMouseHeld = true;
                    if (app->appFlags & AppFlags_SpiralEvent) {
                        if (!spiralGen) {
                            spiralGen = new SpiralGenerator();
                            app->m_rbSystem->addForceGenerator(spiralGen);
                        }
                    } else if (app->appFlags & AppFlags_DragObjectsEvent) {
                    } else if (app->appFlags & AppFlags_SpawnObjectsEvent) {
                        spawnEvent(app);
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    isMouseHeld = false;
                }
                break;
        }
        if (!isMouseHeld) {
            if (spiralGen) {
                app->m_rbSystem->removeForceGenerator(spiralGen);
                spiralGen = nullptr;
            }
        }
        ImGui_ImplSDL2_ProcessEvent(&event);
    }  // event loop
}

int clickedOnObject(Application* app) {
    int x, y = 0;
    SDL_GetMouseState(&x, &y);
    Vec2 mousePos = Vec2{((double)x) / 100, -((double)y) / 100};

    for (auto ob : app->m_objects) {
        if (ob->shape->containsPoint(mousePos)) {
            return ob->idx;
        }
    }
    return -1;
}

void spawnEvent(Application* app) {
    static Uint64 time = 0;
    if (SDL_GetPerformanceCounter() - time > 1500 || time == 0) {
        int x, y = 0;
        SDL_GetMouseState(&x, &y);
        Vec2 mousePos = Vec2{((double)x) / 100, -((double)y) / 100};

        // for randomization
        //  double maxSize = 0.3;
        //   for (auto ob : app->m_objects) {
        //       Vec2 rbPos = ob->shape->rigidbody->pos;
        //       double distance = (mousePos - rbPos).magnitude();
        //       if(distance < maxSize){
        //           maxSize = distance;
        //       }
        //   }
        // double randomSize = ((double)(rand() % (int)(maxSize * 1000)))/1000;
        double randomSize = 0.3;
        Rigidbody* rb = new Rigidbody(mousePos.x, mousePos.y);
        rb->m = 10;
        // random bool
        Shape* sh;
        if (rand() % 2 == 0) {
            sh = new rectangleShape(rb, randomSize, randomSize);
        } else {
            sh = new circleShape(rb, randomSize);
        }
        Object* ob = new Object();
        ob->init(app, sh, Object::DYNAMIC, Color(rand() % 4));
        time = SDL_GetPerformanceCounter();
    }
}
void dragEvent() {}

}  // namespace Events
