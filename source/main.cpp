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
    Rigidbody* rb1 = new Rigidbody(400, -400);
    rectangleShape* r1 = new rectangleShape(400, 100, 13);
    Object* ob1 = new Object();
    ob1->init(app, r1, rb1, Object::FIXED, Object::BLUE);

    Rigidbody* rb2 = new Rigidbody(400, -200);
    rectangleShape* r2 = new rectangleShape(200, 100, 2);
    Object* ob2 = new Object();
    ob2->init(app, r2, rb2, Object::DYNAMIC, Object::PURPLE);

    Rigidbody* rb3 = new Rigidbody(200, -200);
    rectangleShape* r3 = new rectangleShape(50, 20, 5);
    Object* ob3 = new Object();
    ob3->init(app, r3, rb3, Object::DYNAMIC, Object::YELLOW);

    Rigidbody* rb4 = new Rigidbody(700, -100);
    rectangleShape* r4 = new rectangleShape(50, 40, 7);
    Object* ob4 = new Object();
    ob4->init(app, r4, rb4, Object::DYNAMIC, Object::YELLOW);

    rb1->m = 300000000;
    rb2->m = 3000000;
    rb3->m = 30000000;
    rb4->m = 300000;


    r1->rigidbody = rb1;
    r2->rigidbody = rb2;
    r3->rigidbody = rb3;
    r4->rigidbody = rb4;
    //rb2->v.y = -10;
    rb2->theta = 90;

    app->run();

//   SDL_Event e;
//    while (true) {
//        while (SDL_PollEvent(&e)) {
//            if (e.type == SDL_QUIT) {
//                break;
//            }
//        }
//        app->render();
//        SDL_RenderPresent(app->renderer->m_renderer);
 //   }
    // Run the application (starts the main loop)
    // while (true) {
    //    while (SDL_PollEvent(&e)) {
    // if (e.type == SDL_QUIT) {
    // Quit the loop when the user closes the window
    //      break;
    //  }
    //}

    // SDL_SetRenderDrawColor(app->renderer->m_renderer, 255, 255, 255, 255);
    // SDL_RenderClear(app->renderer->m_renderer);
    //  ob1->shape->render(app->renderer, 2);
    //  auto vertices = r1->getVertices();
    //  app->renderer->drawRect(vertices, 3);
    //
    //  r1->render(app->renderer, 3);
    //  ob1->render(app->renderer);
    //  app->render();
    // app->objects[1]->render(app->renderer);

    // std::cout << app->objects.size() << "\n";
    // for (int i = 0; i < app->objects.size(); ++i) {
    //   app->objects[i]->render(app->renderer);
    //}
    // SDL_RenderPresent(app->renderer->m_renderer);
    //    }

    // Clean up and close SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
