#include "../header/Application.hpp"
#include <iostream>
#include <stdexcept>
#include "GravityGenerator.hpp"

Application::Application(SDL_Renderer* _renderer, int _fps = 60) {
    renderer = new Renderer(_renderer);
    fps = _fps;

    rbSystem = new RigidbodySystem();
    GravityGenerator* gGenerator = new GravityGenerator();
    rbSystem->addForceGenerator(gGenerator);

    cResolver = new CollisionResolver();
}

void Application::addObject(Object* object) {
    objects.emplace_back(object);
    object->idx = objects.size() - 1;
}

void Application::render() {
    SDL_SetRenderDrawColor(renderer->m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer->m_renderer);
    for (int i = 0; i < objects.size(); ++i) {
        objects[i]->render(renderer);
    }
    cResolver->renderCollisionPoints(renderer);
}

void Application::run() {
    bool isRunning = true;
    const double expected_frame_time =
        1000.0 / 60.0;
    const double physics_time_step =
        1.0 / 600.0;
    double accumulator = 0.0;

    Uint64 start = SDL_GetPerformanceCounter();
    SDL_Event e;
    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                throw std::runtime_error("EXIT");
                break;
            }
        }
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed = static_cast<double>(current - start) /
                         SDL_GetPerformanceFrequency();
        start = current;
        accumulator += elapsed;


        // Physics update loop
        //std::cout << "physics" << "\n";
        while (accumulator >= physics_time_step) {
            rbSystem->step(physics_time_step);
            cResolver->checkCollisions(this);
            accumulator -= physics_time_step;
        }

        // Render
        render();
        SDL_RenderPresent(renderer->m_renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        double frame_time = static_cast<double>(end - current) /
                            SDL_GetPerformanceFrequency() *
                            1000.0;
        if (frame_time < expected_frame_time) {
            SDL_Delay(static_cast<Uint32>(expected_frame_time - frame_time));
        }
    }
}
