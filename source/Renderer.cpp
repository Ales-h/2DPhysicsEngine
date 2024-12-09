#include "../header/Renderer.hpp"

#include <iostream>

Renderer::Renderer(SDL_Renderer* renderer) {
    m_renderer = renderer;

    m_colors = {
        SDL_Color{70, 255, 178},   // Green
        SDL_Color{70, 255, 1},     // Blue
        SDL_Color{183, 124, 255},  // Purple
        SDL_Color{255, 234, 101}   // Yellow
    };
}

SDL_Color Renderer::getColor(int c) { return m_colors[c]; }

// multiply by 100, make it a int,  1 unit in physics engine = 100 pixels
int Renderer::windowX(const double& x) { return static_cast<int>(x * 100); }

// multiply by 100, make it a int,  1 unit in physics engine = 100 pixels
// make it negative because rendering have flipped y axis
int Renderer::windowY(const double& y) { return static_cast<int>(-y * 100); }

// to convert radius, width, length etc. to a window size
int Renderer::windowLength(const double& a) { return static_cast<int>(a * 100); }

void Renderer::drawRect(std::vector<Vec2>& vertices, int c) {
    SDL_Vertex sdlVertices[4];
    SDL_Color color = getColor(c);

    for (int i = 0; i < 4; ++i) {
        sdlVertices[i].position = {static_cast<float>(windowX(vertices[i].x)),
                                   static_cast<float>(windowY(vertices[i].y))};
        sdlVertices[i].color = color;
    }

    int indices[] = {0, 1, 2, 0, 2, 3};
    SDL_RenderGeometry(m_renderer, nullptr, sdlVertices, 4, indices, 6);
}
void Renderer::drawCircle(Vec2 pos, double radius, int c) {
    SDL_Color color = getColor(c);
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

    int centerX = windowX(pos.x);
    int centerY = windowY(pos.y);
    int x = 0;
    int y = windowLength(radius);
    int d = 1 - windowLength(radius);

    while (x <= y) {
        SDL_RenderDrawLine(m_renderer, centerX - x, centerY - y, centerX + x,
                           centerY - y);
        SDL_RenderDrawLine(m_renderer, centerX - x, centerY + y, centerX + x,
                           centerY + y);
        SDL_RenderDrawLine(m_renderer, centerX - y, centerY - x, centerX + y,
                           centerY - x);
        SDL_RenderDrawLine(m_renderer, centerX - y, centerY + x, centerX + y,
                           centerY + x);

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
    SDL_SetRenderDrawColor(m_renderer, 255, 165, 0, 255);

    const int boxSize = 10;
    for (const Vec2& cp : cps) {
        SDL_Rect rect;
        rect.x = windowX(cp.x) - boxSize / 2;  // Center the box
        rect.y = windowY(cp.y) - boxSize / 2;
        rect.w = boxSize;
        rect.h = boxSize;

        SDL_RenderFillRect(m_renderer, &rect);
    }

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}
