#include "../header/Application.hpp"

#include <iostream>
#include <stdexcept>

#include "GravityGenerator.hpp"
#include "Object.hpp"

Application::Application(int _fps) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        std::exit(-1);
    }

    // Create an SDL window
    SDL_Window* window =
        SDL_CreateWindow("2D Physics Simulation", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        std::exit(-1);
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        SDL_Log("Failed to create m_renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::exit(-1);
    }

    m_renderer = new Renderer(renderer);
    m_window = window;
    fps = _fps;

    m_rbSystem = new RigidbodySystem();
    m_cResolver = new CollisionResolver();
}

Application::~Application(){
    //   TTF_Quit();
    SDL_DestroyRenderer(m_renderer->m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    delete(m_renderer);
    delete(m_rbSystem);
    delete(m_cResolver);
}

void Application::addObject(Object* object) {
    m_objects.emplace_back(object);
    object->idx = m_objects.size() - 1;
}

void Application::render() {
    SDL_SetRenderDrawColor(m_renderer->m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer->m_renderer);
    for (int i = 0; i < m_objects.size(); ++i) {
        m_objects[i]->render(m_renderer);
        if (SDL_GetPerformanceCounter() - startTime > 100 &&
            m_objects[i]->type != Object::FIXED &&
            m_objects[i]->shape->rigidbody->v != Vec2::zero()) {
            m_objects[i]->shape->rigidbody->renderVelocityVector(m_renderer);
        }
    }
    m_cResolver->renderCollisionPoints(m_renderer);
}

void Application::run() {
    bool isRunning = true;
    const double expected_frame_time = 1000.0 / 60.0;
    const double physics_time_step = 1.0 / 600.0;
    double accumulator = 0.0;

    Uint64 start = SDL_GetPerformanceCounter();
    startTime = start;
    SDL_Event e;
    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                std::exit(0);
            }
        }
        Uint64 current = SDL_GetPerformanceCounter();
        double elapsed =
            static_cast<double>(current - start) / SDL_GetPerformanceFrequency();
        start = current;
        accumulator += elapsed;

        // Physics update loop
        // std::cout << "physics" << "\n";
        while (accumulator >= physics_time_step) {
            m_rbSystem->step(physics_time_step);
            m_cResolver->checkCollisions(this);
            accumulator -= physics_time_step;
        }

        // Render
        render();
        SDL_RenderPresent(m_renderer->m_renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        double frame_time =
            static_cast<double>(end - current) / SDL_GetPerformanceFrequency() * 1000.0;
        if (frame_time < expected_frame_time) {
            SDL_Delay(static_cast<Uint32>(expected_frame_time - frame_time));
        }
    }
}
