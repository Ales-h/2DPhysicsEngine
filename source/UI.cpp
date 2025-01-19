#include "UI.hpp"

#include <iostream>
#include <string>

#include "SDL.h"
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
    ImGui::BeginChild("Selector", ImVec2((float)window_width / 2, (float)window_height),
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
            app->m_renderer->m_renderer, SDL_PIXELFORMAT_BGRA8888,
            SDL_TEXTUREACCESS_TARGET, window_width / 2, window_height / 2);
        SDL_Texture* fullsizeTexture = app->renderScenePreview(scenes[selected]);
        // textureScenePreview = fullsizeTexture;
        SDL_SetRenderTarget(app->m_renderer->m_renderer, textureScenePreview);

        SDL_RenderCopy(app->m_renderer->m_renderer, fullsizeTexture, NULL, NULL);
        SDL_SetRenderTarget(app->m_renderer->m_renderer, nullptr);
    }

    ImGui::SameLine();  // Position the second child window next to the first
    ImGui::BeginChild("Preview", ImVec2((float)window_width / 2, (float)window_height),
                      flagsToolbar);
    if (selected != -1) {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%s", scenes[selected]->name.c_str());
        ImGui::SetWindowFontScale(1.f);

        // PREVIEW IMAGE
        ImGui::Image((ImTextureID)textureScenePreview,
                     ImVec2((float)window_width / 2., (float)window_height / 2.));

        // NAME + EDIT NAME
        static char name[128];

        strncpy(name, scenes[selected]->name.c_str(), sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';  // Ensure null-termination

        static bool editing = false;

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
            if (ImGui::Button("Edit")) {
                editing = true;
            }
        }
    }
    ImGui::SetCursorPosX(((float)window_width) / 2. * 1.1);
    ImGui::SetCursorPosY((float)window_height - 100);
    if (ImGui::Button("Select", ImVec2(100, 50))) {
        app->loadScene(scenes[selected]);
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
void renderSettingWindow();
void renderToolsBar();
void renderObjectWindow();

}  // namespace UI
