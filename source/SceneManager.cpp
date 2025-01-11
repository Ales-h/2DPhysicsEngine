#include "SceneManager.hpp"

#include <iostream>

#include "json.hpp"

namespace SceneManager {

nlohmann::json sceneToJSON(Application* app) {
    nlohmann::json scene;

    scene["name"] = app->sceneName;
    for (int i = 0; i < app->m_objects.size(); ++i) {
        Object* ob = app->m_objects[i];
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "object%d", ob->idx);
        std::string name(buffer);

        scene["objects"][name]["id"] = ob->idx;
        scene["objects"][name]["color"] = ob->color;
        scene["objects"][name]["type"] = ob->type;
        scene["objects"][name]["shape"]["type"] = ob->shape->type;
        if (ob->shape->type == 'c') {
            circleShape* circle = static_cast<circleShape*>(ob->shape);
            scene["objects"][name]["shape"]["radius"] = circle->radius;
        } else if (ob->shape->type == 'r') {
            rectangleShape* rect = static_cast<rectangleShape*>(ob->shape);
            scene["objects"][name]["shape"]["width"] = rect->width;
            scene["objects"][name]["shape"]["height"] = rect->height;
        }
        Rigidbody* rb = ob->shape->rigidbody;
        scene["objects"][name]["shape"]["rigidbody"]["pos"] =
            std::string(std::to_string(rb->pos.x) + ';' + std::to_string(rb->pos.y));
        scene["objects"][name]["shape"]["rigidbody"]["v"] =
            std::string(std::to_string(rb->v.x) + ';' + std::to_string(rb->v.y));
        scene["objects"][name]["shape"]["rigidbody"]["a"] =
            std::string(std::to_string(rb->a.x) + ';' + std::to_string(rb->a.y));
        scene["objects"][name]["shape"]["rigidbody"]["m"] = std::to_string(rb->m);
        scene["objects"][name]["shape"]["rigidbody"]["theta"] = std::to_string(rb->theta);
        scene["objects"][name]["shape"]["rigidbody"]["omega"] = std::to_string(rb->omega);
        scene["objects"][name]["shape"]["rigidbody"]["epsilon"] =
            std::to_string(rb->epsilon);
    }
    return scene;
}
void loadScene(Application* app, std::string path);
bool isUniqueName(std::string name) {
    std::string path = "../scenes/userScenes";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (name == entry.path()) {
            return false;
        }
    }
    return true;
}
void saveSceneToFile(Application* app) {
    std::string path = "../scenes/userScenes/";

    auto jsonScene = sceneToJSON(app);
    std::string fileName = app->sceneName;
    int i = 1;
    while (true) {
        i++;
        if (isUniqueName(fileName)) {
            break;
        }
        fileName = fileName + std::to_string(i);
    }
    path += fileName + ".json";
    std::freopen(path.c_str(), "w", stdout);

    std::cout << jsonScene.dump(4);

    freopen("CON", "w", stdout);  // stdout back to console
}

}  // namespace SceneManager
