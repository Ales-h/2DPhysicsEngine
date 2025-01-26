#ifndef PHYSOLVER_RENDERER_HPP
#define PHYSOLVER_RENDERER_HPP
#include <vector>
#include <array>
#include <SDL.h>
#include "Vec2.hpp"
class Scene;

class Renderer {
public:
    SDL_Renderer* sdl_renderer;

    Renderer(SDL_Renderer* renderer);
    SDL_Color getColor(int c);

    int windowX(const double& x);
    int windowY(const double& y);
    int windowLength(const double& a);

    void drawRect(std::array<Vec2, 4>& vertices, int c);
    void drawRectOutline(std::array<Vec2, 4>& vertices, int c);
    void drawCircle(Vec2 pos, double radius, int c);
    void drawCircleOutline(Vec2 pos, double radius, int c);
    void drawCollisionPoints(std::vector<Vec2>& cps);
    void drawArrow(Vec2 pos, Vec2 dir, double magnitude);

private:
    // TODO refactor with array, vector is not needed
    std::vector<SDL_Color> m_colors;

};

#endif
