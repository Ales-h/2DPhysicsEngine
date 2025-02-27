#include "UI.hpp"

#include <iostream>
#include <string>

#include "Application.hpp"
#include "GravityGenerator.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "SceneManager.hpp"
#include "imgui.h"

namespace UI {

const ImGuiWindowFlags flagsToolbar =
    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoScrollbar;

const ImGuiWindowFlags window_flags =
    ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
    ImGuiWindowFlags_NoScrollbar;

// to not create a preview texture every frame we save it
static SDL_Texture* textureScenePreview;
// icon textures (we dont need many icons so this aproach is fine)
static SDL_Texture* pauseTexture;
static SDL_Texture* resumeTexture;
static SDL_Texture* resetTexture;
static SDL_Texture* editTexture;
static SDL_Texture* settingTexture;
static SDL_Texture* moveTexture;
static SDL_Texture* spiralTexture;
static SDL_Texture* spawnTexture;

void initUI(SDL_Renderer* renderer) {
    pauseTexture = loadTexture(renderer, "../assets/icons/pause.png");
    resumeTexture = loadTexture(renderer, "../assets/icons/resume.png");
    resetTexture = loadTexture(renderer, "../assets/icons/reset.png");
    editTexture = loadTexture(renderer, "../assets/icons/edit.png");
    settingTexture = loadTexture(renderer, "../assets/icons/setting.png");
    moveTexture = loadTexture(renderer, "../assets/icons/move.png");
    spiralTexture = loadTexture(renderer, "../assets/icons/magnet.png");
    spawnTexture = loadTexture(renderer, "../assets/icons/spawn.png");
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path) {
    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf) {
        std::cerr << "Failed to load image " << path << ": " << IMG_GetError() << '\n';
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return texture;
}

void renderSceneSelectWindow(Application* app,
                             std::vector<SceneManager::Scene*>& scenes) {
    static bool reloadScenes = false;
    std::string selectedNameAfterChange = "";
    int window_width = 0;
    int window_height = 0;
    SDL_GetWindowSize(app->m_window, &window_width, &window_height);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)window_width, (float)window_height));
    ImGui::Begin("Select a Scene", NULL, window_flags);
    ImGui::BeginChild("Selector",
                      ImVec2((float)window_width * 1. / 3, (float)window_height),
                      window_flags);
    static int selected = -1;
    int previous_selected = selected;
    ImGui::SetWindowFontScale(1.5f);
    for (int n = 0; n < scenes.size(); n++) {
        char buf[32];
        strcpy(buf, scenes[n]->name.c_str());
        if (ImGui::Selectable(buf, selected == n, 0)) selected = n;
    }
    ImGui::SetWindowFontScale(1.f);
    ImGui::EndChild();
    if (previous_selected != selected) {
        if (textureScenePreview != nullptr) {
            SDL_DestroyTexture(textureScenePreview);
        }
        textureScenePreview = SDL_CreateTexture(
            app->m_renderer->sdl_renderer, SDL_PIXELFORMAT_BGRA8888,
            SDL_TEXTUREACCESS_TARGET, window_width / 2, window_height / 2);
        SDL_Texture* fullsizeTexture = app->renderScenePreview(scenes[selected]);
        // textureScenePreview = fullsizeTexture;
        SDL_SetRenderTarget(app->m_renderer->sdl_renderer, textureScenePreview);

        SDL_RenderCopy(app->m_renderer->sdl_renderer, fullsizeTexture, NULL, NULL);
        SDL_SetRenderTarget(app->m_renderer->sdl_renderer, nullptr);
    }

    ImGui::SameLine();  // Position the second child window next to the first
    ImGui::BeginChild("Preview",
                      ImVec2((float)window_width * 2. / 3, (float)window_height),
                      flagsToolbar);
    if (selected != -1) {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%s", scenes[selected]->name.c_str());
        ImGui::SetWindowFontScale(1.f);

        // PREVIEW IMAGE
        ImGui::SetCursorPosX(window_width * 1. / 12);
        ImGui::Image((ImTextureID)textureScenePreview,
                     ImVec2((float)window_width / 2., (float)window_height / 2.));

        // NAME + EDIT NAME
        static char name[128];

        strncpy(name, scenes[selected]->name.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';  // Ensure null-termination

        static bool editing = false;

        ImGui::SetWindowFontScale(2.f);
        ImGui::Text("Name: ");
        ImGui::SameLine();

        if (editing) {
            if (ImGui::InputText("##edit", name, IM_ARRAYSIZE(name),
                                 ImGuiInputTextFlags_EnterReturnsTrue)) {
                // name cant be empty ""
                if (strnlen(name, sizeof(name)) > 0 && scenes[selected]->name != name) {
                    SceneManager::renameFile(scenes[selected]->name, std::string(name));
                    reloadScenes = true;
                    selectedNameAfterChange = std::string(name);
                }
                editing = false;
            }
            if (ImGui::IsItemDeactivated()) {
                editing = false;
            }
        } else {
            ImGui::Text("%s", scenes[selected]->name.c_str());
            ImGui::SameLine();
            if (ImGui::ImageButton("edit", (ImTextureID)editTexture, ImVec2(20, 20))) {
                editing = true;
            }
        }
        ImGui::Text("Objects count: %zu", scenes[selected]->objects.size());

        ImGui::Text("Gravity: ");
        ImGui::SameLine();
        if (scenes[selected]->gravity) {
            ImGui::Text("ON");
        } else {
            ImGui::Text("OFF");
        }
        ImGui::SetWindowFontScale(1.f);
    }
    if (selected == -1) {
        ImGui::BeginDisabled();
    }
    ImGui::SetCursorPosX(((float)window_width) / 2. * 1.1);
    ImGui::SetCursorPosY((float)window_height - 100);
    if (ImGui::Button("Select", ImVec2(100, 50))) {
        app->loadScene(scenes[selected]);
    }
    ImGui::SetCursorPosX(((float)window_width) / 2. * 1.1 - 120);
    ImGui::SetCursorPosY((float)window_height - 100);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1, 52. / 255, 52. / 255, 0.5});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1, 0.3, 0.3, 1});
    if (ImGui::Button("Delete", ImVec2(100, 50))) {
        SceneManager::eraseFile(scenes[selected]->name);
        reloadScenes = true;
        selected = -1;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    if (selected == -1) {
        ImGui::EndDisabled();
    }
    ImGui::EndChild();

    ImGui::End();

    // RELOAD SCENES
    if (reloadScenes) {
        // scenes can load in different order after renaming

        SceneManager::clearScenes(scenes);
        scenes = SceneManager::loadScenes();
        reloadScenes = false;

        // put selected back to the original selected Scene
        for (int i = 0; i < scenes.size(); ++i) {
            if (selectedNameAfterChange == scenes[i]->name) {
                selected = i;
                break;
            }
        }
    }
}
void renderToolbar(Application* app, bool& isSimRunning) {
    ImGui::SetNextWindowPos(ImVec2{0, 15});
    ImGui::SetNextWindowSize(ImVec2{45, 185});
    ImGui::Begin("test", NULL, flagsToolbar);

    if (isSimRunning) {
        if (ImGui::ImageButton("pause", (ImTextureID)pauseTexture, ImVec2(20, 20))) {
            isSimRunning = !isSimRunning;
        }
    } else {
        if (ImGui::ImageButton("resume", (ImTextureID)resumeTexture, ImVec2(20, 20))) {
            isSimRunning = !isSimRunning;
        }
    }
    if (ImGui::ImageButton("reset", (ImTextureID)resetTexture, ImVec2(20, 20))) {
        isSimRunning = false;
        app->loadScene(app->m_scene);
    }

    static ImVec4 selectColor(66. / 255, 150. / 255, 249. / 255, 1);
    static int select = 0;
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::PushStyleColor(ImGuiCol_Button,
                          select == 1 ? selectColor : ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    if (ImGui::ImageButton("drag", (ImTextureID)moveTexture, ImVec2(20, 20))) {
        select = 1;
        app->appFlags &= ~AppFlags_SpiralEvent;
        app->appFlags &= ~AppFlags_SpawnObjectsEvent;
        app->appFlags |= AppFlags_DragObjectsEvent;
    }
    ImGui::PushStyleColor(ImGuiCol_Button,
                          select == 2 ? selectColor : ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    if (ImGui::ImageButton("spiral", (ImTextureID)spiralTexture, ImVec2(20, 20))) {
        select = 2;
        app->appFlags &= ~AppFlags_SpawnObjectsEvent;
        app->appFlags &= ~AppFlags_DragObjectsEvent;
        app->appFlags |= AppFlags_SpiralEvent;
    }
    ImGui::PushStyleColor(ImGuiCol_Button,
                          select == 3 ? selectColor : ImVec4(0, 0, 0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
    if (ImGui::ImageButton("spawn", (ImTextureID)spawnTexture, ImVec2(20, 20))) {
        select = 3;
        app->appFlags &= ~AppFlags_SpiralEvent;
        app->appFlags &= ~AppFlags_DragObjectsEvent;
        app->appFlags |= AppFlags_SpawnObjectsEvent;
    }
    for (int i = 0; i < 3; ++i) {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

void renderMainMenuBar(Application* app, std::vector<SceneManager::Scene*>& scenes,
                       bool& isRunning, bool& showSettings) {
    static bool saveAsWin = false;
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Select a Scene")) {
                app->clear();
                app->m_scene = nullptr;
            }
            if (ImGui::MenuItem("Save")) {
                std::string name = app->m_scene->name;
                SceneManager::eraseFile(app->m_scene->name);
                SceneManager::saveSceneToFile(app);
                SceneManager::clearScenes(scenes);
                scenes = SceneManager::loadScenes();

                for (auto scene : scenes) {
                    if (scene->name == name) {
                        app->m_scene = scene;
                        break;
                    }
                }
            }
            if (ImGui::MenuItem("Save as")) {
                saveAsWin = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Settings")) {
                showSettings = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                isRunning = false;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (saveAsWin) {
        ImGui::OpenPopup("Save As");
    }
    if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char buff[25];

        ImGui::Text("Save As: ");
        ImGui::SameLine();
        ImGui::InputText("##filename", buff, sizeof(buff));

        if (ImGui::Button("Save")) {
            // Perform saving logic
            app->m_scene->name = buff;
            SceneManager::saveSceneToFile(app);
            SceneManager::clearScenes(scenes);
            scenes = SceneManager::loadScenes();
            for (auto scene : scenes) {
                if (scene->name == buff) {
                    app->m_scene = scene;
                    break;
                }
            }
            ImGui::CloseCurrentPopup();
            saveAsWin = false;  // Close the popup
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            saveAsWin = false;
        }
        ImGui::EndPopup();
    }
}

void renderSettingsWindow(Application* app, bool& open) {
    ImGui::Begin("Settings", &open);

    // GRAVITY CHECKBOX
    bool gravity = app->appFlags & AppFlags_Gravity;
    bool gravityChanged = gravity;
    ImGui::Checkbox("gravity", &gravity);
    if (gravity != gravityChanged) {
        app->appFlags ^= AppFlags_Gravity;
        if (!gravity) {
            app->m_rbSystem->removeForceGenerator(app->m_rbSystem->getGravityGenerator());
        } else {
            app->m_rbSystem->addForceGenerator(new GravityGenerator());
        }
    }

    // GRAVITY a
    double* gravA;
    static double gravNull = 0;
    if (!gravity) {
        ImGui::BeginDisabled();
        gravA = &gravNull;
    } else {
        gravA = &app->m_rbSystem->getGravityGenerator()->_g;
    }
    ImGui::Text("Gravitational accelaration");
    ImGui::SameLine();
    // TODO : remake the tooltip with ImGui::HelpMarker
    ImGui::Text("(?)");
    ImGui::SetItemTooltip(
        "Gravitacni zrychleni je hodnota, ktera telesa urychluje na ose y");
    ImGui::InputDouble("###gravityacceleration", gravA, 0.01f, 1.0f, "%.8f");
    if (!gravity) {
        ImGui::EndDisabled();
    }
    ImGui::Text("Elasticity");
    ImGui::SliderFloat("###elasticity", &app->m_cResolver->_e, 0.0f, 1.0f, "%.3f");

    ImGui::Text("Static Friction");
    ImGui::SliderFloat("###staticfriction", &app->m_cResolver->staticFriction, 0.0f, 1.0f,
                       "%.3f");

    ImGui::Text("Dynamic Friction");
    ImGui::SliderFloat("###dynamicfriction", &app->m_cResolver->dynamicFriction, 0.0f,
                       app->m_cResolver->staticFriction, "%.3f");

    static bool ShowCP = app->appFlags & AppFlags_ShowCollisionPoints;
    bool ShowCPChanged = ShowCP;
    ImGui::Checkbox("Show collision points", &ShowCP);
    if (ShowCP != ShowCPChanged) {
        app->appFlags ^= AppFlags_ShowCollisionPoints;
    }
    // TODO Tooltips
    ImGui::SameLine();
    ImGui::Text("(?)");
    ImGui::SetItemTooltip("");

    static bool ShowVV = app->appFlags & AppFlags_ShowVelocityVectors;
    bool ShowVVChanged = ShowVV;
    ImGui::Checkbox("Show velocity vectors", &ShowVV);
    if (ShowVV != ShowVVChanged) {
        app->appFlags ^= AppFlags_ShowVelocityVectors;
    }
    ImGui::SameLine();
    ImGui::Text("(?)");
    ImGui::SetItemTooltip("");

    ImGui::End();
}

void renderObjectSettingsButton(std::vector<int>& objectWinIndices, int selected,
                                Vec2 pos) {
    // converting simulation coordinates to screen coordinates
    Vec2 screenPos = Vec2{pos.x * 100 - 5, -pos.y * 100 - 5};
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::SetNextWindowPos(ImVec2{(float)screenPos.x, (float)screenPos.y});
    ImGui::SetNextWindowSize(ImVec2{10, 10});
    ImGui::Begin("Object Setting button", NULL,
                 flagsToolbar | ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
    if (ImGui::ImageButton("setting", (ImTextureID)settingTexture, ImVec2(10, 10))) {
        if (std::find(objectWinIndices.begin(), objectWinIndices.end(), selected) ==
            objectWinIndices.end()) {
            objectWinIndices.push_back(selected);
        }
    }
    ImGui::PopStyleVar();
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();
}

void renderObjectWindow(Application* app, int& selected, const int objectIdx,
                        bool& open) {
    assert(app->m_objects.size() > 0);
    Object* ob = app->m_objects[objectIdx];
    Shape* shape = ob->shape;
    Rigidbody* rb = shape->rigidbody;
    char buf[25];
    snprintf(buf, sizeof(buf), "Object %d", objectIdx);
    ImGui::Begin(buf, &open);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{1, 0, 0, 1});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{1, 0.3, 0.3, 1});
    if (ImGui::Button("Remove", ImVec2{ImGui::GetContentRegionAvail().x - 20, 30})) {
        open = false;
        app->removeObject(app->m_objects[objectIdx]);
        selected = -1;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();

    ImGui::Text("Shape");
    const char* shapes[] = {"Circle", "Rectangle"};
    int currentShape = (shape->type == 'c') ? 0 : 1;
    ImGui::Combo("Shape", &currentShape, shapes, IM_ARRAYSIZE(shapes));
    if (currentShape == 0 && shape->type != 'c') {
        auto rect = dynamic_cast<rectangleShape*>(shape);
        Shape* newShape = new circleShape(rb, std::max(rect->width, rect->height));
        shape->rigidbody = nullptr;  // we dont want to free the rb
        delete shape;
        ob->shape = newShape;
        shape = newShape;
    } else if (currentShape == 1 && shape->type != 'r') {
        auto circle = dynamic_cast<circleShape*>(shape);
        Shape* newShape = new rectangleShape(rb, circle->radius * 2, circle->radius * 2);
        shape->rigidbody = nullptr;  // we dont want to free the rb
        delete shape;
        ob->shape = newShape;
        shape = newShape;
    }
    if (shape->type == 'c') {
        auto circle = dynamic_cast<circleShape*>(shape);
        ImGui::Text("Radius: ");
        ImGui::SameLine();
        ImGui::InputDouble("###radius", &circle->radius);
    } else if (shape->type == 'r') {
        auto rect = dynamic_cast<rectangleShape*>(shape);
        ImGui::Text("Width: ");
        ImGui::SameLine();
        ImGui::InputDouble("###width", &rect->width);
        ImGui::Text("Height: ");
        ImGui::SameLine();
        ImGui::InputDouble("###height", &rect->height);
    }

    ImGui::Text("Object %d Data", objectIdx);

    ImGui::Combo("Color", (int*)&ob->color, colorNames, IM_ARRAYSIZE(colorNames));

    // OBJECT TYPE
    const char* obTypes[] = {"Fixed", "Dynamic"};
    int obTypeChanged = ob->type;
    ImGui::Combo("Object Type", (int*)&ob->type, obTypes, IM_ARRAYSIZE(obTypes));
    if (obTypeChanged != ob->type) {
        if (ob->type == Object::FIXED) {
            app->m_rbSystem->removeRigidbody(rb);
            rb->m = INFINITY;
        } else {
            rb->m = 1;
            app->m_rbSystem->addRigidbody(rb);
        }
    }

    if (ob->type == Object::FIXED) {
        ImGui::BeginDisabled();
    }
    ImGui::Text("Mass: ");
    ImGui::SameLine();
    ImGui::InputDouble("###mass", &rb->m);
    if (ob->type == Object::FIXED) {
        ImGui::EndDisabled();
    }
    // Vector input
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float inputWidth = ((totalWidth * 0.9f - ImGui::CalcTextSize("x: y:").x) / 2.0f);

    ImGui::Text("Position [m]");
    ImGui::Text("x:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###posx", &rb->pos.x);

    ImGui::SameLine();
    ImGui::Text("y:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###posy", &rb->pos.y);

    ImGui::Text("Velocity [m/s]");
    ImGui::Text("x:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###velx", &rb->v.x);

    ImGui::SameLine();
    ImGui::Text("y:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###vely", &rb->v.y);

    ImGui::Text("Accelaration [m/s^2]");
    ImGui::Text("x:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###accx", &rb->a.x);

    ImGui::SameLine();
    ImGui::Text("y:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth);
    ImGui::InputDouble("###accy", &rb->a.y);

    ImGui::Text("");
    ImGui::Text("Rotation");
    // THETA
    ImGui::Text("theta: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth * 1.5);
    ImGui::InputDouble("###theta", &rb->theta);
    ImGui::SameLine();
    ImGui::Text("rad");
    // OMEGA
    ImGui::Text("omega: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth * 1.5);
    ImGui::InputDouble("###omega", &rb->omega);
    ImGui::SameLine();
    ImGui::Text("rad/s");
    // EPSILON
    ImGui::Text("epsilon: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(inputWidth * 1.5);
    ImGui::InputDouble("###epsilon", &rb->epsilon);
    ImGui::SameLine();
    ImGui::Text("rad/s^2");

    ImGui::End();
}

}  // namespace UI
