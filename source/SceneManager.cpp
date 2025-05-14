#include "SceneManager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "json.hpp"

namespace SceneManager {

std::vector<Scene*> loadScenes() {
    std::vector<Scene*> scenes;
    for (const auto& entry : std::filesystem::directory_iterator(SCENES_PATH)) {
        std::ifstream f(entry.path());
        nlohmann::json data = nlohmann::json::parse(f);
        f.close();

        // if file name and name in json are not equal we fix it
        std::string jsonName = data["name"];
        std::string fileName = entry.path().stem().string();
        if (jsonName != fileName) {
            data["name"] = fileName;
            std::ofstream outFile(entry.path());
            outFile << data.dump(4);
            outFile.close();
        }
        scenes.push_back(new Scene(JSONToScene(data)));
    }
    return scenes;
}

void clearScenes(std::vector<Scene*> scenes) {
    for (Scene* sc : scenes) {
        delete sc;
    }
    scenes.clear();
}



void eraseFile(std::string name) {
    std::string path = SCENES_PATH.string() + name + ".json";
    
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
        std::cout << "file erased: " << path << '\n';
    } else {
        std::cerr << "file does not exist: " << path << '\n';
    }
}

void renameFile(std::string oldName, std::string newName) {
    std::string path = "";
    for (const auto& entry : std::filesystem::directory_iterator(SCENES_PATH)) {
        if ((oldName + ".json") == entry.path().filename()) {
            path = entry.path().string();
            break;
        }
    }
    // read the json
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Error: failed to open a Scene file: " + path);
    }
    nlohmann::json json = nlohmann::json::parse(inputFile);
    inputFile.close();

    // change the name inside the json
    json["name"] = newName;

    // output the json back to the file
    std::ofstream outputFile(path);
    if (!outputFile.is_open()) {
        throw std::runtime_error("Error: failed to open a Scene file: " + path);
    }

    outputFile << json.dump(4);
    outputFile.close();

    // rename the file
    std::string newPath = SCENES_PATH.string() + newName + ".json";
    std::filesystem::rename(path, newPath);
}

Scene JSONToScene(nlohmann::json json) {
    Scene scene;

    scene.name = json["name"];
    int objectsCount = json["objectCount"];
    scene.gravity = json["gravity"];
    scene.objects.reserve(objectsCount);
    for (int i = 0; i < objectsCount; ++i) {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "object%d", i);
        std::string name(buffer);

        // OBJECT
        Object* ob = new Object();
        ob->color = json["objects"][name]["color"];
        ob->idx = json["objects"][name]["id"];
        ob->type = json["objects"][name]["type"];

        // RIGIDBODY
        Rigidbody* rb = new Rigidbody();
        rb->pos = JSONformatToVec(json["objects"][name]["shape"]["rigidbody"]["pos"]);
        rb->v = JSONformatToVec(json["objects"][name]["shape"]["rigidbody"]["v"]);
        rb->a = JSONformatToVec(json["objects"][name]["shape"]["rigidbody"]["a"]);
        rb->m = std::stod(
            json["objects"][name]["shape"]["rigidbody"]["m"].get<std::string>());
        rb->theta = json["objects"][name]["shape"]["rigidbody"]["theta"];
        rb->omega = json["objects"][name]["shape"]["rigidbody"]["omega"];
        rb->epsilon = json["objects"][name]["shape"]["rigidbody"]["epsilon"];

        // SHAPE
        char shapeType = json["objects"][name]["shape"]["type"].template get<char>();
        if (shapeType == 'r') {
            double width = json["objects"][name]["shape"]["width"];
            double height = json["objects"][name]["shape"]["height"];
            Shape* shape = new rectangleShape(rb, width, height);
            ob->shape = shape;
        } else if (shapeType == 'c') {
            double radius = json["objects"][name]["shape"]["radius"];
            Shape* shape = new circleShape(rb, radius);
            ob->shape = shape;
        }
        scene.objects.push_back(ob);
    }
    return scene;
}

Vec2 JSONformatToVec(std::string jsonString) {
    double x = std::stod(jsonString.substr(0, jsonString.find(';')));
    double y = std::stod(jsonString.substr(jsonString.find(';') + 1, jsonString.npos));
    return Vec2{x, y};
}

nlohmann::json sceneToJSON(Application* app) {
    nlohmann::json scene;

    scene["objectCount"] = app->m_objects.size();
    if (app->appFlags & AppFlags_Gravity) {
        scene["gravity"] = true;
    } else {
        scene["gravity"] = false;
    }
    for (int i = 0; i < app->m_objects.size(); ++i) {
        Object* ob = app->m_objects[i];
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "object%d", ob->idx);
        std::string name(buffer);

        scene["objects"][name]["id"] = ob->idx;
        scene["objects"][name]["color"] = ob->color;
        scene["objects"][name]["type"] = ob->type;

        // SHAPE
        if (ob->shape->type == 'c') {
            circleShape* circle = static_cast<circleShape*>(ob->shape);
            scene["objects"][name]["shape"]["type"] = circle->type;
            scene["objects"][name]["shape"]["radius"] = circle->radius;
        } else if (ob->shape->type == 'r') {
            rectangleShape* rect = static_cast<rectangleShape*>(ob->shape);
            scene["objects"][name]["shape"]["type"] = rect->type;
            scene["objects"][name]["shape"]["width"] = rect->width;
            scene["objects"][name]["shape"]["height"] = rect->height;
        }

        // RIGIDBODY
        Rigidbody* rb = ob->shape->rigidbody;
        scene["objects"][name]["shape"]["rigidbody"]["pos"] =
            std::string(std::to_string(rb->pos.x) + ';' + std::to_string(rb->pos.y));
        scene["objects"][name]["shape"]["rigidbody"]["v"] =
            std::string(std::to_string(rb->v.x) + ';' + std::to_string(rb->v.y));
        scene["objects"][name]["shape"]["rigidbody"]["a"] =
            std::string(std::to_string(rb->a.x) + ';' + std::to_string(rb->a.y));
        // because mass can be inf we need to make it a string in JSON
        scene["objects"][name]["shape"]["rigidbody"]["m"] = std::to_string(rb->m);
        scene["objects"][name]["shape"]["rigidbody"]["theta"] = rb->theta;
        scene["objects"][name]["shape"]["rigidbody"]["omega"] = rb->omega;
        scene["objects"][name]["shape"]["rigidbody"]["epsilon"] = rb->epsilon;
    }
    return scene;
}
bool isUniqueName(std::string name) {
    for (const auto& entry : std::filesystem::directory_iterator(SCENES_PATH)) {
        if ((name + ".json") == entry.path().filename()) {
            return false;
        }
    }
    return true;
}
void saveSceneToFile(Application* app) {
    auto jsonScene = sceneToJSON(app);
    std::string fileName = app->m_scene->name;
    int i = 1;
    while (true) {
        i++;
        if (isUniqueName(fileName)) {
            break;
        }
        fileName = fileName + std::to_string(i);
    }
    jsonScene["name"] = fileName;  // file name and scene name is the same
    fileName = SCENES_PATH.string() + fileName + ".json";
    std::cout << "saving to      " << fileName << '\n';
    std::freopen(fileName.c_str(), "w", stdout);

    std::cout << jsonScene.dump(4);

    freopen("CON", "w", stdout);  // stdout back to console
}

}  // namespace SceneManager
