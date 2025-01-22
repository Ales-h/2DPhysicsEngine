#ifndef PHYSOLVER_UI
#define PHYSOLVER_UI

#include "Application.hpp"
#include "SceneManager.hpp"

namespace UI {


void initUI(SDL_Renderer* renderer); // initiliazes the icon textures with SDL
SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path);

void renderSceneSelectWindow(Application* app, std::vector<SceneManager::Scene*>& scenes);
void renderSettingWindow(Application* app);
void renderToolsBar(Application* app, bool& isSimRunning);
void renderObjectWindow();

}

#endif
