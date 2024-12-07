#ifndef PHYSOLVER_RENDERER_HPP
#define PHYSOLVER_RENDERER_HPP
#include <vector>
#include <SDL.h>
#include "Vec2.hpp"

class Renderer {
public:
    SDL_Renderer* m_renderer;

    Renderer(SDL_Renderer* renderer);
    SDL_Color getColor(int c);
    void drawRect(std::vector<Vec2>& vertices, int color);
    void drawCircle(Vec2 pos, int radius, int color);
    void drawCollisionPoints(std::vector<Vec2>& cps);

private:
    std::vector<SDL_Color> m_colors;

};

#endif
