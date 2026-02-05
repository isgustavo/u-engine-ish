#include "UEngine.h"
#include "UScene.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <string>
#include "UEditorScene.h"
#include "editor/Editor_ComponentRegistry.h"
#include "components/CTransform.h"
#include <components/CPath.h>
#include <components/CSprite.h>
#include <components/CTarget.h>
#include <components/CAnimation.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

namespace uei
{
    UEngine::~UEngine()
    {
        if (editor_Prefab != nullptr)
        {
            delete editor_Prefab;
            editor_Prefab = nullptr;
        }
    }
    uei::UAsset& UEngine::Assets()
    {
        return *assets;
    }
    sf::RenderWindow& UEngine::RenderWindow()
    {
        return renderWindow;
    }
    sf::Vector2i& UEngine::ScreenSize()
    {
        return screenSize;
    }
    void UEngine::Start()
    {
        //uei::CSprite c;
        //if (scenes.empty()) return;
        //currentScene = scenes.back().get();
        //if (currentScene != nullptr)
        //    currentScene->Start();
    }
  
    void UEngine::Update()
    {
        sf::CircleShape shape(100.f);
        shape.setFillColor(sf::Color::Green);

        sf::Clock deltaClock;
        while (renderWindow.isOpen())
        {
            while (const auto event = renderWindow.pollEvent())
            {
                ImGui::SFML::ProcessEvent(renderWindow, *event);

                if (event->is<sf::Event::Closed>())
                {
                    renderWindow.close();
                }
            }

            ImGui::SFML::Update(renderWindow, deltaClock.restart());
            
            if (bShowEditor)
            {
                Editor_Show();
            }

            if(currentScene != nullptr)
                currentScene->Update();

            renderWindow.clear();
            
            if (currentScene != nullptr)
                currentScene->Draw();

            //if (bShowGrid)
            //{
            //    renderWindow.setView(gridView);
            //    //renderWindow.setView(gridView);
            //    sf::RectangleShape rect = sf::RectangleShape({ 90.0f, 90.f });
            //    rect.setFillColor(sf::Color::White);
            //    renderWindow.draw(rect);
            //    //ShowGrid();
            //    gridView.move(sf::Vector2f(1.f, 1.f));
            //    //renderWindow.setView(renderWindow.getDefaultView());
            //}

            ImGui::SFML::Render(renderWindow);
            renderWindow.display();
        }

        ImGui::SFML::Shutdown();
    }

    void UEngine::LoadAsset()
    {
        assets->LoadFromFile(editor_levelName);
    }

    void UEngine::Editor_ShowFileTab()
    {
        if (ImGui::BeginTabItem("File"))
        {
            if (!editor_bLevelLoaded)
            {
                if (!editor_bNewLevel)
                {
                    ImGui::Text("Level Name:");
                    ImGui::SameLine();
                    ImGui::InputText("##levelName", editor_levelName, IM_ARRAYSIZE(editor_levelName));
                    if (ImGui::Button("Open/Create"))
                        Editor_LoadLevel();
                }
                else
                {
                    ImGui::Text("Grid Column:");
                    ImGui::SameLine();
                    ImGui::InputInt("##GridColumn", &editor_gridColumn);

                    ImGui::Text("Grid Row:");
                    ImGui::SameLine();
                    ImGui::InputInt("##GridRow", &editor_gridRow);

                    ImGui::Text("Grid Size:");
                    ImGui::SameLine();
                    ImGui::InputInt("##GridSize", &editor_gridSize);

                    if (ImGui::Button("Create"))
                        Editor_CreateLevel();
                }
            }
            else
            {
                ImGui::Text(("Level Name: " + std::string(editor_levelName)).c_str());

                if (ImGui::Button("Save"))
                {
                    //SaveLevel();
                }
                if (ImGui::Button("Save/Play"))
                {
                    //SaveLevel();
                    //PlayLevel();
                }
                if (ImGui::Button("Save/Close"))
                {
                    //SaveLevel();
                    //CloseLevel();
                }
                if (ImGui::Button("Close"))
                {
                    //CloseLevel();
                }
            }

            ImGui::EndTabItem();
        }
    }
    void UEngine::Editor_ShowPrefabGallery()
    {
        const float thumbnailSize = 64.0f;
        const float padding = 12.0f;
        const float cellSize = thumbnailSize + padding;

        ImGui::BeginChild("EntityGallery", ImVec2(0, 0), true);

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = std::max(1, (int)(panelWidth / cellSize));

        ImGui::Columns(columns, nullptr, false);
        int i = 0;
        for (auto& [prefabName, value] : currentScene->Prefabs())
        {
            if (editor_Prefab != nullptr && prefabName == editor_Prefab->Name()) continue;

            ImGui::PushID(prefabName.c_str());

            if (value.get()->HasComponent<CSprite>())
            {
                CSprite* sprite = value.get()->GetComponent<CSprite>();
                auto& editor_data = Assets().Sprite(sprite->SpriteName());
                if (sprite->Sprite() == nullptr)
                {
                    sprite->Sprite(new sf::Sprite(Assets().GetTexture(editor_data.textureName),
                        sf::IntRect({ editor_data.x, editor_data.y }, { editor_data.width, editor_data.height })));
                }

                ImTextureID id = (ImTextureID)(intptr_t)sprite->Sprite()->getTexture().getNativeHandle();
                sf::Vector2u size = sprite->Sprite()->getTexture().getSize();

                ImVec2 uv0(
                    (float)editor_data.x / size.x,
                    (float)editor_data.y / size.y);

                ImVec2 uv1(
                    (float)(editor_data.x + editor_data.width) / size.x,
                    (float)(editor_data.y + editor_data.height) / size.y);

                ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
            }
            else
            {
                if (ImGui::Button("##thumb" + (++i), ImVec2(thumbnailSize, thumbnailSize)))
                {

                }
            }

            if (ImGui::IsItemHovered())
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                {
                    editor_Prefab = value.get();
                    editor_bShowEditPrefab = true;
                }
                else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    std::cout << "LEFT" << std::endl;
                }
            }

            ImGui::TextWrapped("%s", prefabName.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }
    }
    void UEngine::Editor_ShowPrefabTab()
    {
        if (editor_bLevelLoaded)
        {
            if (ImGui::BeginTabItem("Prefabs"))
            {
                if (ImGui::Button("New Prefab"))
                {
                    Editor_NewPrefab();
                }

                ImGui::Spacing();

                Editor_ShowPrefabGallery();

                ImGui::Columns(1);
                ImGui::EndChild();
                ImGui::EndTabItem();
            }
        }
    }
    void UEngine::Editor_ShowNewPrefab()
    {
        ImGui::Begin("New Prefab");
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::InputText("##PrefabName", editor_prefabName, IM_ARRAYSIZE(editor_prefabName));
        if (!std::string(editor_prefabName).empty())
        {
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                editor_Prefab->Name(std::string(editor_prefabName));
                Editor_SavePrefab();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close"))
            {
                editor_bShowNewPrefab = false;
            }
        }

        int selectedIndex = 0;
        ImGui::Text("Add Components:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##AddComponents", " "))
        {
            for (int i = 0; i < Editor_ComponentRegistry::AllComponents().size(); ++i) {
                const bool isSelected = (selectedIndex == i);

                if (Editor_ComponentRegistry::AllComponents()[i] == "CTransform") continue;
                if (ImGui::Selectable(Editor_ComponentRegistry::AllComponents()[i].c_str(), isSelected))
                {
                    editor_Prefab->AddComponent(Editor_ComponentRegistry::Create(Editor_ComponentRegistry::AllComponents()[i]));
                }
            }
            ImGui::EndCombo();
        }

        int i = 0;
        for (auto& [type, value] : editor_Prefab->Components())
        {
            value->Editor_Show(*this, true);
        }

        ImGui::End();
    }
    void UEngine::Editor_ShowEditPrefab()
    {
        ImGui::Begin("Edit Prefab");
        ImGui::Text(("Prefab: " + std::string(editor_Prefab->Name())).c_str());
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            Editor_SaveUpdatePrefab();
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            editor_Prefab = nullptr;
            editor_bShowEditPrefab = false;
        }
        else
        {
            int selectedIndex = 0;
            ImGui::Text("Add Components:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##AddComponents", " "))
            {
                for (int i = 0; i < Editor_ComponentRegistry::AllComponents().size(); ++i) {
                    const bool isSelected = (selectedIndex == i);

                    if (Editor_ComponentRegistry::AllComponents()[i] == "CTransform") continue;
                    if (ImGui::Selectable(Editor_ComponentRegistry::AllComponents()[i].c_str(), isSelected))
                    {
                        editor_Prefab->AddComponent(Editor_ComponentRegistry::Create(Editor_ComponentRegistry::AllComponents()[i]));
                    }
                }
                ImGui::EndCombo();
            }

            int i = 0;
            for (auto& [type, value] : editor_Prefab->Components())
            {
                value->Editor_Show(*this, true);
            }
        }

        ImGui::End();
    }
    void UEngine::Editor_Show()
    {
        ImGui::Begin("Editor");
        if (ImGui::BeginTabBar("u-engine-ish"))
        {
            Editor_ShowFileTab();
            Editor_ShowPrefabTab();

            ImGui::EndTabBar();
        }

        if (editor_bShowNewPrefab)
        {
            Editor_ShowNewPrefab();
        }

        if (editor_bShowEditPrefab)
        {
            Editor_ShowEditPrefab();
        }

        ImGui::End();
    }

    void UEngine::Editor_NewPrefab()
    {
        editor_bShowNewPrefab = true;
        if (editor_Prefab != nullptr)
        {
            delete editor_Prefab;
            editor_Prefab = nullptr;
        }
        editor_Prefab = new UEntity();
        editor_Prefab->AddComponent<CTransform>(sf::Vector2f(0.f, 0.f));
    }
    void UEngine::Editor_SaveUpdatePrefab()
    {
        Editor_SaveLevel();
        editor_bShowEditPrefab = false;
        editor_Prefab = nullptr;
    }
    void UEngine::Editor_SavePrefab()
    {
        currentScene->Prefabs().emplace(editor_Prefab->Name(), editor_Prefab->Clone());
        Editor_SaveLevel();
        editor_bShowEditPrefab = false;
        editor_Prefab = nullptr;
        editor_bShowNewPrefab = false;
    }
    void UEngine::Editor_CreateLevel()
    {
        std::string levelPath = "Assets/" + std::string(editor_levelName) + ".txt";
        std::ofstream file(levelPath);
        if (!file)
        {
            std::cerr << "Failed to create Assets/"<< editor_levelName <<".txt" << std::endl;
            return;
        }

        file << LEVEL << " " << editor_gridColumn << " " << editor_gridRow << " " << editor_gridSize << " " << "\n";
        file.close();
        Editor_LoadLevel();
    }
    void UEngine::Editor_LoadLevel()
    {
        scenes.clear();
        assets->LoadFromFile("");
        AddScene<uei::UEditorScene>();
        currentScene = scenes.back().get();
        currentScene->Start(editor_levelName);
        editor_bLevelLoaded = true;
        editor_bNewLevel = false;
    }
    void UEngine::Editor_SaveLevel()
    {
        std::string levelPath = "Assets/" + std::string(editor_levelName) + ".txt";
        if (std::filesystem::exists(levelPath))
        {
            std::string content;
            content.clear();

            content = LEVEL + " " + std::to_string(editor_gridColumn) + " " + std::to_string(editor_gridRow) + " " + std::to_string(editor_gridSize) + "\n";
        
            for (const auto& [key, value] : currentScene->Prefabs())
            {
                content += PREFAB + " " + value->Name() + "\n";

                for (const auto& [c_key, c_value] : value->components)
                {
                    content += COMPONENT + " " + c_value->Editor_ComponentName() + " " + c_value->Editor_Save() + "\n";
                }
            }

            std::ofstream out(levelPath);
            out << content;
            out.close();
        }
    }
}

//REGISTER_COMPONENT(CTarget);
//REGISTER_COMPONENT(CAnimation);
REGISTER_COMPONENT(CSprite);
REGISTER_COMPONENT(CTransform);