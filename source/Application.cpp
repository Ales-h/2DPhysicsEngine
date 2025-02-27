#include "../header/Application.hpp"

#include <SDL_render.h>
#include <SDL_timer.h>
#include <SDL_video.h>

#include <iostream>
#include <stdexcept>

#include "Events.hpp"
#include "GravityGenerator.hpp"
#include "Object.hpp"
#include "SceneManager.hpp"
#include "UI.hpp"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "imgui.h"

Application::Application(int _fps) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        std::exit(-1);
    }

    // Create an SDL window
    SDL_Window* window =
        SDL_CreateWindow("2D Physics Simulation", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 1200, 800, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        std::exit(-1);
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
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
    appFlags = 0;
    appFlags |= AppFlags_ShowCollisionPoints | AppFlags_ShowVelocityVectors;
    m_scene = nullptr;
}

Application::~Application() {
    //   TTF_Quit();
    SDL_DestroyRenderer(m_renderer->sdl_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    delete (m_renderer);
    delete (m_rbSystem);
    delete (m_cResolver);
}

void Application::addObject(Object* object) {
    m_objects.emplace_back(object);
    object->idx = m_objects.size() - 1;
}

void Application::removeObject(Object* object) {
    // swap with last element and resize
    m_objects[object->idx] = m_objects.back();
    m_objects[object->idx]->idx = object->idx;
    m_objects.resize(m_objects.size() - 1);
}

void Application::loadScene(SceneManager::Scene* scene) {
    clear();
    m_scene = scene;
    m_objects.reserve(scene->objects.size());
    for (auto ob : scene->objects) {
        Object* obCopy = new Object(*ob);
        if (obCopy->type != Object::FIXED) {
            m_rbSystem->addRigidbody(obCopy->shape->rigidbody);
        }
        addObject(obCopy);
    }
    if (scene->gravity) {
        appFlags |= AppFlags_Gravity;
        GravityGenerator* gg = new GravityGenerator();
        m_rbSystem->addForceGenerator(gg);
    }
}

void Application::clear() {
    // clear objects
    for (Object* ob : m_objects) {
        delete ob;
    }
    m_objects.clear();

    // delete RbSystem
    delete m_rbSystem;
    m_rbSystem = new RigidbodySystem();

    appFlags = 0;
    appFlags |= AppFlags_ShowCollisionPoints | AppFlags_ShowVelocityVectors;
}

SDL_Texture* Application::renderScenePreview(SceneManager::Scene* scene) {
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(m_window, &window_width, &window_height);
    SDL_Texture* texture =
        SDL_CreateTexture(m_renderer->sdl_renderer, SDL_PIXELFORMAT_BGRA8888,
                          SDL_TEXTUREACCESS_TARGET, window_width, window_height);

    SDL_SetRenderTarget(m_renderer->sdl_renderer, texture);
    SDL_SetRenderDrawColor(m_renderer->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer->sdl_renderer);
    for (int i = 0; i < scene->objects.size(); ++i) {
        scene->objects[i]->render(m_renderer);
        if (scene->objects[i]->type != Object::FIXED &&
            scene->objects[i]->shape->rigidbody->v != Vec2::zero()) {
            scene->objects[i]->shape->rigidbody->renderVelocityVector(m_renderer);
        }
    }
    SDL_SetRenderTarget(m_renderer->sdl_renderer, nullptr);
    return texture;
}

void Application::render() {
    SDL_SetRenderDrawColor(m_renderer->sdl_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer->sdl_renderer);
    for (int i = 0; i < m_objects.size(); ++i) {
        m_objects[i]->render(m_renderer);
        if (SDL_GetPerformanceCounter() - startTime > 100 &&
            m_objects[i]->type != Object::FIXED &&
            m_objects[i]->shape->rigidbody->v != Vec2::zero()) {
            if (appFlags & AppFlags_ShowVelocityVectors) {
                m_objects[i]->shape->rigidbody->renderVelocityVector(m_renderer);
            }
        }
    }
    if (appFlags & AppFlags_ShowCollisionPoints) {
        m_cResolver->renderCollisionPoints(m_renderer);
    } else {
    }
}

void Application::run() {
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    bool isRunning = true;
    bool isSimulationRunning = false;
    bool showSimSettings = true;

    const double expected_frame_time = 1000.0 / 60.0;
    const double physics_time_step = 1.0 / 600.0;
    double accumulator = 0.0;

    Uint64 start = SDL_GetPerformanceCounter();
    Uint64 current;
    startTime = start;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // io.FontGlobalScale = 2.0f;
    (void)io;

    // if(isSimulationRunning) then selectedObjectID = -1 also when nothing is selected
    int selectedObjectID = -1;
    // Indices of objects to render a ObjectSettingsWindow
    std::vector<int> selectedWinIndices;
    std::vector<SceneManager::Scene*> scenes = SceneManager::loadScenes();
    UI::initUI(m_renderer->sdl_renderer);

    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer->sdl_renderer);
    ImGui_ImplSDLRenderer2_Init(m_renderer->sdl_renderer);
    while (isRunning) {
        if (isSimulationRunning) {
            appFlags |= AppFlags_iSSimulationRunning;
        } else {
            appFlags &= ~AppFlags_iSSimulationRunning;
        }
        Events::handleEvents(this, isRunning, selectedObjectID);

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (isSimulationRunning) {
            current = SDL_GetPerformanceCounter();
            double elapsed =
                static_cast<double>(current - start) / SDL_GetPerformanceFrequency();
            start = current;
            accumulator += elapsed;

            // Physics update loop
            while (accumulator >= physics_time_step) {
                m_rbSystem->step(physics_time_step);
                m_cResolver->checkCollisions(this);
                accumulator -= physics_time_step;
            }
        }
        bool ifChanged = isSimulationRunning;
        UI::renderToolbar(this, isSimulationRunning);
        UI::renderMainMenuBar(this, scenes, isRunning, showSimSettings);
        if (ifChanged != isSimulationRunning) {
            start = SDL_GetPerformanceCounter();
        }
        if (showSimSettings) {
            UI::renderSettingsWindow(this, showSimSettings);
        }

        if (m_scene == nullptr) {
            UI::renderSceneSelectWindow(this, scenes);
        }

        if (m_objects.size() > 0) {
            // iterating backwards to avoid shifting issues
            for (int i = selectedWinIndices.size() - 1; i >= 0; --i) {
                bool isOpen = true;
                UI::renderObjectWindow(this, selectedObjectID,  selectedWinIndices[i], isOpen);
                if (!isOpen) {
                    selectedWinIndices.erase(selectedWinIndices.begin() + i);
                }
            }
        }
        render();  // Simulation Render (SDL)
        if (selectedObjectID != -1) {
            Shape* tmp = m_objects[selectedObjectID]->shape;
            m_objects[selectedObjectID]->shape->renderOutline(m_renderer, Color::RED);
            UI::renderObjectSettingsButton(selectedWinIndices, selectedObjectID,
                                           tmp->rigidbody->pos);
        }
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(),
                                              m_renderer->sdl_renderer);
        SDL_RenderPresent(m_renderer->sdl_renderer);

        Uint64 end = SDL_GetPerformanceCounter();
        if (isSimulationRunning) {
            double frame_time = static_cast<double>(end - current) /
                                SDL_GetPerformanceFrequency() * 1000.0;
            if (frame_time < expected_frame_time) {
                SDL_Delay(static_cast<Uint32>(expected_frame_time - frame_time));
            }
        }
    }
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}
