#include <SDL.h>

#include <iostream>

#include "../header/Application.hpp"

int main() {
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

    // Creating objects
    Rigidbody* rb1 = new Rigidbody(4, -6);
    rectangleShape* r1 = new rectangleShape(7, 1, 13);
    Object* ob1 = new Object();
    ob1->init(app, r1, rb1, Object::FIXED, Object::BLUE);

    Rigidbody* rb2 = new Rigidbody(5, 4);
    rectangleShape* r2 = new rectangleShape(1, 1, 2);
    Object* ob2 = new Object();
    ob2->init(app, r2, rb2, Object::DYNAMIC, Object::PURPLE);

    Rigidbody* rb3 = new Rigidbody(2.5, -2);
    rectangleShape* r3 = new rectangleShape(0.5, 0.2, 5);
    Object* ob3 = new Object();
    ob3->init(app, r3, rb3, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb4 = new Rigidbody(7, -1);
    circleShape* s4 = new circleShape(0.5);
    Object* ob4 = new Object();
    ob4->init(app, s4, rb4, Object::DYNAMIC, Object::YELLOW);


    rb1->m = 200;
    rb2->m = 40;
    rb3->m = 30;
    rb4->m = 40;


    r1->rigidbody = rb1;
    r2->rigidbody = rb2;
    r3->rigidbody = rb3;
    s4->rigidbody = rb4;
    //rb2->v.y = -10;
    rb2->theta = 2.1;
    rb3->theta = 1;

    app->run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
