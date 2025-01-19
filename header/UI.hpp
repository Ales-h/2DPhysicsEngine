#ifndef PHYSOLVER_UI
#define PHYSOLVER_UI

#include "Application.hpp"
#include "SceneManager.hpp"

namespace UI {

void renderSceneSelectWindow(Application* app, std::vector<SceneManager::Scene*>& scenes);
void renderSettingWindow();
void renderToolsBar();
void renderObjectWindow();

}

#endif
