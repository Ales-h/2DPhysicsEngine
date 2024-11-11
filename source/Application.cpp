#include "../header/Application.hpp"

Application::Application(SDL_Renderer* _renderer, int _fps = 60){
    renderer = new Renderer(_renderer);
    fps = _fps;

    rbSystem = new RigidbodySystem();

    cResolver = new CollisionResolver();
}

void Application::addObject(Object* object){
    objects.emplace_back(object);
    object->idx = objects.size()-1;
}

void Application::run(){
    bool isRunning = true;

    while(isRunning){

    }
}
