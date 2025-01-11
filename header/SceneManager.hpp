#ifndef PHYSOLVER_SCENE_MANAGER_HPP
#define PHYSOLVER_SCENE_MANAGER_HPP

#include "json.hpp"
#include "Application.hpp"

namespace SceneManager {

nlohmann::json sceneToJSON(Application* app);
void loadScene(Application* app, std::string path);
bool isUniqueName(std::string);
void saveSceneToFile(Application* app);

}  // namespace SceneManager

#endif
