#include "Renderer.hpp"

#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"

#include <cmath>
#include <iostream>

SDL_FColor getColor(int c) { return sdlColors[c]; }

Renderer::Renderer(SDL_Renderer* renderer) { sdl_renderer = renderer; }

// multiply by 100, make it a int,  1 unit in physics engine = 100 pixels
int Renderer::windowX(const double& x) { return static_cast<int>(x * 100); }

// multiply by 100, make it a int,  1 unit in physics engine = 100 pixels
// make it negative because rendering have flipped y axis
int Renderer::windowY(const double& y) { return static_cast<int>(-y * 100); }

// to convert radius, width, length etc. to a window size
int Renderer::windowLength(const double& a) { return static_cast<int>(a * 100); }

void Renderer::drawRect(std::array<Vec2, 4>& vertices, int c) {
    if (vertices.size() != 4) {
        throw std::runtime_error("drawRect requires exactly 4 vertices");
    }
    SDL_Vertex sdlVertices[4];
    SDL_FColor color = getColor(c);

    for (int i = 0; i < 4; ++i) {
        sdlVertices[i].position = {static_cast<float>(windowX(vertices[i].x)),
                                   static_cast<float>(windowY(vertices[i].y))};
        sdlVertices[i].color = color;
    }

    int indices[] = {0, 1, 2, 0, 2, 3};
    bool success = SDL_RenderGeometry(sdl_renderer, nullptr, sdlVertices, 4, indices, 6);

    if(!success){
        std::cerr << "Rendering rectangle error: " << SDL_GetError() << '\n';
    }
}

void Renderer::drawRectOutline(std::array<Vec2, 4>& vertices, int c) {
    SDL_FColor color = getColor(c);
    SDL_SetRenderDrawColorFloat(sdl_renderer, color.r , color.g , color.b , color.a);

    SDL_FPoint sdlPoints[5];
    for (int i = 0; i < 4; ++i) {
        sdlPoints[i].x = static_cast<float>(windowX(vertices[i].x));
        sdlPoints[i].y = static_cast<float>(windowY(vertices[i].y));
    }
    sdlPoints[4] = sdlPoints[0];
    SDL_RenderLines(sdl_renderer, &sdlPoints[0], 5);
}

void Renderer::drawCircle(Vec2 pos, double radius, double theta, int c) {
    SDL_FColor color = getColor(c);
    std::cout << color.r * 255 << ' ' << color.g << '\n';
    SDL_SetRenderDrawColorFloat(sdl_renderer, color.r , color.g , color.b , color.a);

    int centerX = windowX(pos.x);
    int centerY = windowY(pos.y);
    int x = 0;
    int y = windowLength(radius);
    int d = 1 - windowLength(radius);

    while (x <= y) {
        SDL_RenderLine(sdl_renderer, centerX - x, centerY - y, centerX + x,
                           centerY - y);
        SDL_RenderLine(sdl_renderer, centerX - x, centerY + y, centerX + x,
                           centerY + y);
        SDL_RenderLine(sdl_renderer, centerX - y, centerY - x, centerX + y,
                           centerY - x);
        SDL_RenderLine(sdl_renderer, centerX - y, centerY + x, centerX + y,
                           centerY + x);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
    double r = windowLength(radius);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 125);
    SDL_RenderLine(sdl_renderer, centerX, centerY, centerX + std::sin(theta) * r,
                       centerY + std::cos(theta) * r);
}
void Renderer::drawCircleOutline(Vec2 pos, double radius, int c) {
    SDL_FColor color = getColor(c);
    SDL_SetRenderDrawColorFloat(sdl_renderer, color.r , color.g , color.b , color.a);

    int centerX = windowX(pos.x);
    int centerY = windowY(pos.y);
    int x = 0;
    int y = windowLength(radius);
    int d = 1 - windowLength(radius);

    while (x <= y) {
        SDL_RenderPoint(sdl_renderer, centerX + x, centerY + y);
        SDL_RenderPoint(sdl_renderer, centerX - x, centerY + y);
        SDL_RenderPoint(sdl_renderer, centerX + x, centerY - y);
        SDL_RenderPoint(sdl_renderer, centerX - x, centerY - y);
        SDL_RenderPoint(sdl_renderer, centerX + y, centerY + x);
        SDL_RenderPoint(sdl_renderer, centerX - y, centerY + x);
        SDL_RenderPoint(sdl_renderer, centerX + y, centerY - x);
        SDL_RenderPoint(sdl_renderer, centerX - y, centerY - x);

        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void Renderer::drawCollisionPoints(std::vector<Vec2>& cps) {
    SDL_SetRenderDrawColor(sdl_renderer, 255, 165, 0, 255);
    const int boxSize = 10;
    for (const Vec2& cp : cps) {
        SDL_FRect rect;
        rect.x = windowX(cp.x) - boxSize / 2;  // Center the box
        rect.y = windowY(cp.y) - boxSize / 2;
        rect.w = boxSize;
        rect.h = boxSize;

        SDL_RenderFillRect(sdl_renderer, &rect);
    }

    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
}

void Renderer::drawArrow(Vec2 pos, Vec2 dir, double magnitude) {
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
    Vec2 normalizedDir = dir.normalize();
    Vec2 end = {pos.x + normalizedDir.x * magnitude, pos.y + normalizedDir.y * magnitude};

    SDL_RenderLine(sdl_renderer, windowX(pos.x), windowY(pos.y), windowX(end.x),
                       windowY(end.y));

    constexpr double arrowHeadLength = 0.1;
    constexpr double arrowHeadAngle = 3.1415926 / 6;

    Vec2 left = {end.x - arrowHeadLength * (std::cos(arrowHeadAngle) * normalizedDir.x -
                                            std::sin(arrowHeadAngle) * normalizedDir.y),
                 end.y - arrowHeadLength * (std::sin(arrowHeadAngle) * normalizedDir.x +
                                            std::cos(arrowHeadAngle) * normalizedDir.y)};

    Vec2 right = {
        end.x - arrowHeadLength * (std::cos(-arrowHeadAngle) * normalizedDir.x -
                                   std::sin(-arrowHeadAngle) * normalizedDir.y),
        end.y - arrowHeadLength * (std::sin(-arrowHeadAngle) * normalizedDir.x +
                                   std::cos(-arrowHeadAngle) * normalizedDir.y)};

    SDL_RenderLine(sdl_renderer, windowX(end.x), windowY(end.y), windowX(left.x),
                       windowY(left.y));
    SDL_RenderLine(sdl_renderer, windowX(end.x), windowY(end.y), windowX(right.x),
                       windowY(right.y));
}
