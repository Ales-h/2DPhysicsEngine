#include <SDL.h>
// #include <SDL_ttf.h>

#include <iostream>
#include <stdexcept>

#include "../header/Scenes.hpp"
#include "../header/ballsScene.hpp"
#include "../header/gravityScene.hpp"
#include "Application.hpp"
#include "SceneManager.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "You have to write the Scene number to the argument\n";
        std::exit(-1);
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return -1;
    }

    Application* app = new Application();
    //
    //    int sceneNumber = std::atoi(argv[1]);
    //
    //    if (sceneNumber == 1) {
    //        gravityScene* scene = new gravityScene(app);
    //        scene->init();
    //    } else if (sceneNumber == 2) {
    //        ballsScene* scene = new ballsScene(app);
    //        scene->init();
    //    } else {
    //        throw std::runtime_error("No scene");
    //    }
    //    SceneManager::saveSceneToFile(app);
    app->run();

    return 0;
}
