#include "../header/Renderer.hpp"

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

void Renderer::drawRect(std::vector<Vec2> vertices, int c){
    SDL_Vertex sdlVertices[4];
    SDL_Color color = getColor(c);

    for(int i = 0; i < 4; ++i){
    sdlVertices[i].position = {static_cast<float>(vertices[i].x), static_cast<float>(vertices[i].y)};
    sdlVertices[i].color = color;
    }

    int indices[] = {0, 1, 2, 0, 2, 3};
    SDL_RenderGeometry(m_renderer, nullptr, sdlVertices, 4, indices, 6);
}
void Renderer::drawCircle(Vec2 pos, int radius, int color){
}
