#ifndef PHYSOLVER_SCENE_MANAGER_HPP
#define PHYSOLVER_SCENE_MANAGER_HPP

#include "json.hpp"

namespace SceneManager {

nlohmann::json sceneToJSON();
void loadScene();
void saveSceneToFile();


}  // namespace SceneManager

#endif
