#ifndef PHYSOLVER_SCENE_MANAGER_HPP
#define PHYSOLVER_SCENE_MANAGER_HPP

#include <vector>

#include "Application.hpp"
#include "json.hpp"

namespace SceneManager {

struct Scene {
    std::string name;
    bool gravity = false;
    std::vector<Object*> objects;
};

static std::filesystem::path SCENES_PATH = std::filesystem::path("../scenes/userScenes/");

std::vector<Scene*> loadScenes();
void clearScenes(std::vector<Scene*> scenes);               // proper clean up

void eraseFile(std::string name);
void renameFile(std::string oldName, std::string newName);  // name without prefix (.json)

Scene JSONToScene(nlohmann::json json);
Vec2 JSONformatToVec(std::string jsonString);
nlohmann::json sceneToJSON(Application* app);
bool isUniqueName(std::string);
void saveSceneToFile(Application* app);

}  // namespace SceneManager

#endif
