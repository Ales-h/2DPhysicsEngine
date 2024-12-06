#include "../header/Renderer.hpp"
#include <iostream>

Renderer::Renderer(SDL_Renderer* renderer) {
    m_renderer  = renderer;

    m_colors = {
    SDL_Color{70, 255, 178}, // Green
    SDL_Color{70, 255, 1}, // Blue
    SDL_Color{183, 124, 255}, // Purple
    SDL_Color{255, 234, 101} // Yellow
    };
}

SDL_Color Renderer::getColor(int c){
    return m_colors[c];
}

void Renderer::drawRect(std::vector<Vec2>& vertices, int c){
    SDL_Vertex sdlVertices[4];
    SDL_Color color = getColor(c);

    for(int i = 0; i < 4; ++i){
    sdlVertices[i].position = {static_cast<float>(vertices[i].x), static_cast<float>(-vertices[i].y)};
    sdlVertices[i].color = color;
    }

    int indices[] = {0, 1, 2, 0, 2, 3};
    SDL_RenderGeometry(m_renderer, nullptr, sdlVertices, 4, indices, 6);
    //std::cout << "shape rendered\n";
}
void Renderer::drawCircle(Vec2 pos, int radius, int color){
}

void Renderer::drawCollisionPoints(std::vector<Vec2>& cps){
    SDL_SetRenderDrawColor(m_renderer, 255, 165, 0, 255);

    const int boxSize = 10;
    for (const Vec2& cp : cps) {
        SDL_Rect rect;
        rect.x = static_cast<int>(cp.x) - boxSize / 2; // Center the box
        rect.y = -static_cast<int>(cp.y) - boxSize / 2;
        rect.w = boxSize;
        rect.h = boxSize;

        SDL_RenderFillRect(m_renderer, &rect);
    }

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}




