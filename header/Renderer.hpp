#ifndef PHYSOLVER_RENDERER_HPP
#define PHYSOLVER_RENDERER_HPP
#include <SDL.h>
#include <SDL_pixels.h>

#include <array>
#include <vector>

#include "Vec2.hpp"

// Color refactor
enum Color { GREEN, BLUE, PURPLE, YELLOW, RED, SIZE };

constexpr SDL_Color sdlColors[SIZE] = {
    SDL_Color{70, 255, 178},   // Green
    SDL_Color{68, 200, 252},     // Blue
    SDL_Color{183, 124, 255},  // Purple
    SDL_Color{255, 234, 101},  // Yellow
    SDL_Color{255, 52, 52}       // Red
};

constexpr const char* colorNames[SIZE] = {
    "Green", "Blue", "Purple", "Yellow", "Red",
};

SDL_Color getColor(int c);

class Renderer {
   public:
    SDL_Renderer* sdl_renderer;

    Renderer(SDL_Renderer* renderer);

    int windowX(const double& x);
    int windowY(const double& y);
    int windowLength(const double& a);

    void drawRect(std::array<Vec2, 4>& vertices, int c);
    void drawRectOutline(std::array<Vec2, 4>& vertices, int c);
    void drawCircle(Vec2 pos, double radius, int c);
    void drawCircleOutline(Vec2 pos, double radius, int c);
    void drawCollisionPoints(std::vector<Vec2>& cps);
    void drawArrow(Vec2 pos, Vec2 dir, double magnitude);

};

#endif
