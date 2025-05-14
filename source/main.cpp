#include "SDL3/SDL.h"
// #include <SDL_ttf.h>

#include <iostream>
#include <stdexcept>

#include "Application.hpp"
#include "SceneManager.hpp"

int main(int argc, char* args[]) {


    Application* app = new Application();
    app->run();

    return 0;
}
