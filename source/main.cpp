#include <SDL.h>
//#include <SDL_ttf.h>

#include <iostream>
#include <stdexcept>

#include "../header/Application.hpp"
#include "../header/Scenes.hpp"
#include "../header/ballsScene.hpp"
#include "../header/gravityScene.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "You have to write the Scene number to the argument\n";
        std::exit(-1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    // Create an SDL window
    SDL_Window* window =
        SDL_CreateWindow("2D Physics Simulation", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }
 //   if (TTF_Init() == -1) {
 //       std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
 //       return -1;
 //   }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    Application* app =
        new Application(renderer, 60);  // Pass the renderer and set the FPS

    int sceneNumber = std::atoi(argv[1]);

    if (sceneNumber == 1) {
        gravityScene* scene = new gravityScene(app);
        scene->init();
    } else if (sceneNumber == 2) {
        ballsScene* scene = new ballsScene(app);
        scene->init();
    } else {
        throw std::runtime_error("No scene");
    }

    app->run();

 //   TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
