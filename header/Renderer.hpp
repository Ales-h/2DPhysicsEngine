#ifndef PHYSOLVER_RENDERER_HPP
#define PHYSOLVER_RENDERER_HPP
#include "SDL.h"
#include "rectangleShape.hpp"

class Renderer {
public:
    Renderer(SDL_Renderer* renderer);
    SDL_Color colors(int c);
    void drawRect();
    void drawCircle();

private:
    SDL_Renderer* m_renderer;

};

#endif
