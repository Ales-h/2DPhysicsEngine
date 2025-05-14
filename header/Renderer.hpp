#ifndef PHYSOLVER_RENDERER_HPP
#define PHYSOLVER_RENDERER_HPP
#include "SDL3/SDL.h"
#include "SDL3/SDL_pixels.h"

#include <array>
#include <vector>

#include "Vec2.hpp"

// Color refactor
enum Color { GREEN, BLUE, PURPLE, YELLOW, RED, SIZE };

constexpr SDL_FColor sdlColors[SIZE] = {
    SDL_FColor{70.0f / 255.0f, 255.0f / 255.0f, 178.0f / 255.0f, 1.0f}, // Green
    SDL_FColor{68.0f / 255.0f, 200.0f / 255.0f, 252.0f / 255.0f, 1.0f}, // Blue
    SDL_FColor{183.0f / 255.0f, 124.0f / 255.0f, 255.0f / 255.0f,
               1.0f}, // Purple
    SDL_FColor{255.0f / 255.0f, 234.0f / 255.0f, 101.0f / 255.0f,
               1.0f},                                                 // Yellow
    SDL_FColor{255.0f / 255.0f, 52.0f / 255.0f, 52.0f / 255.0f, 1.0f} // Red
};

constexpr const char *colorNames[SIZE] = {
    "Green", "Blue", "Purple", "Yellow", "Red",
};

SDL_FColor getColor(int c);

class Renderer {
public:
  SDL_Renderer *sdl_renderer;

  Renderer(SDL_Renderer *renderer);

  int windowX(const double &x);
  int windowY(const double &y);
  int windowLength(const double &a);

  void drawRect(std::array<Vec2, 4> &vertices, int c);
  void drawRectOutline(std::array<Vec2, 4> &vertices, int c);
  void drawCircle(Vec2 pos, double radius, double theta, int c);
  void drawCircleOutline(Vec2 pos, double radius, int c);
  void drawCollisionPoints(std::vector<Vec2> &cps);
  void drawArrow(Vec2 pos, Vec2 dir, double magnitude);
};

#endif
