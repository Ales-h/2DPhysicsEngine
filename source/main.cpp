#include <SDL.h>
// #include <SDL_ttf.h>

#include <iostream>
#include <stdexcept>

#include "Application.hpp"
#include "SceneManager.hpp"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    Application* app = new Application();
    app->run();

    return 0;
}
