#include "UEditorScene.h"
#include "UEditor.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include <components/CTransform.h>
#include <components/CSprite.h>
#include <systems/SDrawSystem.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>

namespace uei
{
    UEditorScene::UEditorScene(UEngine& inEngine) : UScene(inEngine) { }

    UEditorScene::~UEditorScene()
    {
        delete prefab;
        prefab = nullptr;
    }

    void UEditorScene::OnStart()
    {
        bMainTab = true;
        AddSystem<SDrawSystem>();

        std::cout << "OnStart" << std::endl;
    }

    void UEditorScene::OnUpdate()
    {
        if (bMainTab)
        {
            ImGui::Begin("Editor");
            if (ImGui::BeginTabBar("u-engine-ish"))
            {
                ShowAssetsTab();
                ShowPrefabTab();
                ShowSceneTab();
                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        if (bShowNewAsset)
        {
            ShowNewAssetTab();
        }

        if (bShowNewPrefab)
        {
            ShowNewPrefabTab();
        }

        if (bShowEditPrefab)
        {
            ShowEditPrefabTab();
        }

        if (bPrefabSelected)
        {
            CTransform* transform = prefab->GetComponent<CTransform>();
            CSprite* sprite = prefab->GetComponent<CSprite>();
            sf::Vector2i localPosition = sf::Mouse::getPosition(engine.RenderWindow());
            int x = ((localPosition.x / gridSize) * gridSize) - (sprite->FlipX() ? -gridSize : 0);
            transform->SetPosition(sf::Vector2f(x, (localPosition.y / gridSize) * gridSize));
        }
    }

    void UEditorScene::OnDraw()
    {
        if (bSceneLoaded)
            DrawGrid();

        engine.RenderWindow().setView(view);

        if (bPrefabSelected)
        {
            CTransform* transform = prefab->GetComponent<CTransform>();
            CSprite* sprite = prefab->GetComponent<CSprite>();
            
            //uei::UAsset* assets = engine.Assets();
            //if (sprite->Sprite() == nullptr)
            //{
            //    auto& data = assets->Sprite(sprite->SpriteName());
            //    sprite->Sprite(new sf::Sprite(assets->GetTexture(data.textureName),
            //        sf::IntRect({ data.x, data.y }, { data.width, data.height })));
            //}

            sf::Sprite* sp = sprite->Sprite();
            sp->setScale(sf::Vector2f((sprite->FlipX() ? -1 : 1) * (gridSize / sp->getTextureRect().size.x), gridSize / sp->getTextureRect().size.y));
            sp->setPosition(transform->Position());

            engine.RenderWindow().draw(*sp);
        }
            
        for (auto& s : systems)
        {
            s.get()->Update(engine, entities);
        }
    }

    void UEditorScene::ShowSceneTab()
    {
        if (ImGui::BeginTabItem("Scene"))
        {
            if (!bSceneLoaded)
            {
                ShowOpenCreateTab();
            }
            else
            {
                ShowSceneOpenTab();
            }

            ImGui::EndTabItem();
        }
    }
    void UEditorScene::ShowOpenCreateTab()
    {
        if (!bNewScene)
        {
            ImGui::Text("Scene Name:");
            ImGui::SameLine();
            ImGui::InputText("##sceneName", editorNameBuffer, IM_ARRAYSIZE(editorNameBuffer));
            if (ImGui::Button("Open/Create"))
                LoadScene();
        }
        else
        {
            ImGui::Text("Grid Column:");
            ImGui::SameLine();
            ImGui::InputInt("##GridColumn", &newSceneGridColumn);

            ImGui::Text("Grid Row:");
            ImGui::SameLine();
            ImGui::InputInt("##GridRow", &newSceneGridRow);

            ImGui::Text("Grid Size:");
            ImGui::SameLine();
            ImGui::InputInt("##GridSize", &newSceneGridSize);

            if (ImGui::Button("Create"))
                CreateScene();
        }
    }
    void UEditorScene::ShowSceneOpenTab()
    {
        ImGui::Text(("Scene Name: " + std::string(editorNameBuffer)).c_str());

        if (ImGui::Button("Save"))
        {
            SaveScene();
        }
        if (ImGui::Button("Save/Play"))
        {
            //SaveLevel();
            //PlayLevel();
        }
        if (ImGui::Button("Save/Close"))
        {
            SaveScene();
            CloseScene();
        }
        if (ImGui::Button("Close"))
        {
            CloseScene();
        }
    }

    void UEditorScene::ShowAssetsTab()
    {
        if (ImGui::BeginTabItem("Assets"))
        {
            if (ImGui::Button("New Asset"))
            {
                bMainTab = false;
                bShowNewAsset = true;
            }

            ImGui::Spacing();

            ShowAssetsGallery();

            ImGui::Columns(1);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
    }
    void UEditorScene::ShowNewAssetTab()
    {
        auto& allAsstTypes = engine.Assets()->AllAssetTypes();
        auto& allTexture = engine.Assets()->AllTextures();

        ImGui::Begin("New Asset");
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::InputText("##AssetName", editorNameBuffer, IM_ARRAYSIZE(editorNameBuffer));
        if (!std::string(editorNameBuffer).empty())
        {
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                bShowNewAsset = false;
                if (allAsstTypes[assetTypeSelectedIndex] == SPRITE)
                {
                    engine.Assets()->AddSprite(editorNameBuffer, engine.Assets()->AllTextures()[assetTextureSelectedIndex], newSpriteX, newSpriteY,
                        newSpriteWidth, newSpriteHeight);
                }
                else if (allAsstTypes[assetTypeSelectedIndex] == ANIMATION)
                {
                    engine.Assets()->AddAnimation(editorNameBuffer, engine.Assets()->AllTextures()[assetTextureSelectedIndex], newSpriteX, newSpriteY,
                        newSpriteWidth, newSpriteHeight, newAnimationFrame, newAnimationSpeed);
                }
                engine.Assets()->Save();
                ClearEditor();
                bMainTab = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            bShowNewAsset = false;
            ClearEditor();
            bMainTab = true;
        }

        ImGui::Text("Asset Type:");
        ImGui::SameLine();
        
        if (ImGui::BeginCombo("##AddAssetType", allAsstTypes[assetTypeSelectedIndex].c_str()))
        {
            for (int i = 0; i < allAsstTypes.size(); ++i)
            {
                const bool isSelected = (assetTypeSelectedIndex == i);
                if (ImGui::Selectable(allAsstTypes[i].c_str(), isSelected))
                {
                    assetTypeSelectedIndex = i;
                }

                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (assetTypeSelectedIndex > 0)
        {
            ImGui::Text("Texture:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##AddTextureType", allTexture[assetTextureSelectedIndex].c_str()))
            {
                for (int i = 0; i < allTexture.size(); ++i)
                {
                    const bool isSelectedTexture = (assetTextureSelectedIndex == i);
                    if (ImGui::Selectable(allTexture[i].c_str(), isSelectedTexture))
                    {
                        assetTextureSelectedIndex = i;
                    }

                    if (isSelectedTexture)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            if (assetTextureSelectedIndex >= 0)
            {
                ImGui::Text("Start X");
                ImGui::SameLine();
                ImGui::PushItemWidth(60);
                ImGui::InputInt("##AddTextureStartX", &newSpriteX, 0);
                ImGui::PopItemWidth();

                ImGui::SameLine();
                
                ImGui::Text("Start Y");
                ImGui::SameLine();
                ImGui::PushItemWidth(60);
                ImGui::InputInt("##AddTextureStartY", &newSpriteY, 0);
                ImGui::PopItemWidth();
                
                ImGui::Text("Width");
                ImGui::SameLine();
                ImGui::PushItemWidth(60);
                ImGui::InputInt("##AddTextureWidth", &newSpriteWidth, 0);
                ImGui::PopItemWidth();
                
                ImGui::SameLine();
                
                ImGui::Text("Height");
                ImGui::SameLine();
                ImGui::PushItemWidth(60);
                ImGui::InputInt("##AddTextureHeight", &newSpriteHeight, 0);
                ImGui::PopItemWidth();
                
                if (allAsstTypes[assetTypeSelectedIndex] == SPRITE)
                {
                    sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(allTexture[assetTextureSelectedIndex]),
                        sf::IntRect({ newSpriteX, newSpriteY }, { newSpriteWidth, newSpriteHeight }));
                    sprite->scale(sf::Vector2f(10.f, 10.f));

                    ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
                    sf::Vector2u size = sprite->getTexture().getSize();

                    ImGui::Spacing();

                    ImVec2 uv0(
                        (float)newSpriteX / size.x,
                        (float)newSpriteY / size.y);

                    ImVec2 uv1(
                        (float)(newSpriteX + newSpriteWidth) / size.x,
                        (float)(newSpriteY + newSpriteHeight) / size.y);

                    ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
                }
                else if (allAsstTypes[assetTypeSelectedIndex] == ANIMATION)
                {
                    ImGui::Text("Frame");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(60);
                    ImGui::InputInt("##AddNewFrame", &newAnimationFrame, 0);
                    ImGui::PopItemWidth();

                    ImGui::SameLine();

                    ImGui::Text("Speed");
                    ImGui::SameLine();
                    ImGui::PushItemWidth(60);
                    ImGui::InputInt("##AddNewSpeed", &newAnimationSpeed, 0);
                    ImGui::PopItemWidth();

                    //ImGui::SliderInt("##CurrentFrame", &currentAnimationFrame, 0, newAnimationFrame - 1);

                    if (newAnimationFrame > 0 && newAnimationSpeed > 0)
                    {
                        currentAnimationDeltaTime += ImGui::GetIO().DeltaTime;
                        currentAnimationFrame = (int)(currentAnimationDeltaTime * newAnimationSpeed) % newAnimationFrame;
                        int spriteX = newSpriteX + (currentAnimationFrame * newSpriteWidth);
                        sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(allTexture[assetTextureSelectedIndex]),
                            sf::IntRect({ spriteX, newSpriteY }, { newSpriteWidth, newSpriteHeight }));
                        sprite->scale(sf::Vector2f(10.f, 10.f));

                        ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
                        sf::Vector2u size = sprite->getTexture().getSize();

                        ImGui::Spacing();

                        ImVec2 uv0(
                            (float)spriteX / size.x,
                            (float)newSpriteY / size.y);

                        ImVec2 uv1(
                            (float)(spriteX + newSpriteWidth) / size.x,
                            (float)(newSpriteY + newSpriteHeight) / size.y);

                        ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
                    }
                }
            }
        }

        ImGui::End();
    }
    void UEditorScene::ShowAssetsGallery() 
    {
        const float thumbnailSize = 64.0f;
        const float padding = 12.0f;
        const float cellSize = thumbnailSize + padding;

        ImGui::BeginChild("AssetGallery", ImVec2(0, 0), true);

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = std::max(1, (int)(panelWidth / cellSize));

        ImGui::Columns(columns, nullptr, false);
        int i = 0;
        for (auto& [key, value] : engine.Assets()->Sprites())
        {
            ImGui::PushID(key.c_str());

            auto& data = engine.Assets()->Sprite(value.name);
            sf::Sprite sprite = sf::Sprite(engine.Assets()->GetTexture(data.textureName),
                sf::IntRect({ data.x, data.y }, { data.width, data.height }));

            ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
            sf::Vector2u size = sprite.getTexture().getSize();

            ImVec2 uv0((float)data.x / size.x, (float)data.y / size.y);
            ImVec2 uv1((float)(data.x + data.width) / size.x, (float)(data.y + data.height) / size.y);

            ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);

            ImGui::TextWrapped("%s", value.name.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }
        currentAnimationDeltaTime += ImGui::GetIO().DeltaTime;
        for (auto& [key, value] : engine.Assets()->Animations())
        {
            ImGui::PushID(key.c_str());

            auto& data = engine.Assets()->Animation(value.name);
            int currentAnimationFrame = (int)(currentAnimationDeltaTime * data.speed) % data.frame;
            int spriteX = data.x + (currentAnimationFrame * data.width);

            sf::Sprite sprite = sf::Sprite(engine.Assets()->GetTexture(data.textureName),
                sf::IntRect({ spriteX, data.y }, { data.width, data.height }));

            ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
            sf::Vector2u size = sprite.getTexture().getSize();

            ImVec2 uv0((float)spriteX / size.x, (float)data.y / size.y);
            ImVec2 uv1((float)(spriteX + data.width) / size.x, (float)(data.y + data.height) / size.y);

            ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);

            ImGui::TextWrapped("%s", value.name.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }
    }

    void UEditorScene::ShowPrefabTab()
    {
        if (ImGui::BeginTabItem("Prefabs"))
        {
            if (ImGui::Button("New Prefab"))
            {
                bMainTab = false;
                prefab = new UEntity();
                prefab->AddComponent<CTransform>(sf::Vector2f(0.f, 0.f));
                bShowNewPrefab = true;
            }

            ImGui::Spacing();

            ShowPrefabGallery();

            ImGui::Columns(1);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
    }
    void UEditorScene::ShowNewPrefabTab()
    {
        ImGui::Begin("New Prefab");
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::InputText("##PrefabName", editorNameBuffer, IM_ARRAYSIZE(editorNameBuffer));
        if (!std::string(editorNameBuffer).empty())
        {
            ImGui::SameLine();
            if (ImGui::Button("Save"))
            {
                bShowNewPrefab = false;
                engine.Assets()->AddPrefab(prefab->Name(), prefab->Clone());
                engine.Assets()->Save();
                ClearEditor();
                bMainTab = true;
                return;
            }
            ImGui::SameLine();
            if (ImGui::Button("Close"))
            {
                bShowNewPrefab = false;
                ClearEditor();
                bMainTab = true;
                return;
            }
        }

        int selectedIndex = 0;
        ImGui::Text("Add Components:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##AddComponents", " "))
        {
            for (int i = 0; i < UEditor::AllComponents().size(); ++i) {
                const bool isSelected = (selectedIndex == i);

                if (UEditor::AllComponents()[i] == "CTransform") continue;
                if (ImGui::Selectable(UEditor::AllComponents()[i].c_str(), isSelected))
                {
                    prefab->AddComponent(UEditor::Create(UEditor::AllComponents()[i]));
                }
            }
            ImGui::EndCombo();
        }

        int i = 0;
        for (auto& [type, value] : prefab->Components())
        {
            value->ShowEditor(engine, true);
        }

        ImGui::End();
    }
    void UEditorScene::ShowEditPrefabTab()
    {
        ImGui::Begin("Edit Prefab");
        ImGui::Text(("Prefab: " + std::string(prefab->Name())).c_str());
        ImGui::SameLine();
        if (ImGui::Button("Save"))
        {
            bShowEditPrefab = false;
            SaveScene();
            //delete prefab;
            prefab = nullptr;
            bMainTab = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            bShowEditPrefab = false;
            //delete prefab;
            prefab = nullptr;
            bMainTab = true;
        }
        
        if(prefab != nullptr)
        {
            int selectedIndex = 0;
            ImGui::Text("Add Components:");
            ImGui::SameLine();
            if (ImGui::BeginCombo("##AddComponents", " "))
            {
                for (int i = 0; i < UEditor::AllComponents().size(); ++i) {
                    const bool isSelected = (selectedIndex == i);

                    if (UEditor::AllComponents()[i] == "CTransform") continue;
                    if (ImGui::Selectable(UEditor::AllComponents()[i].c_str(), isSelected))
                    {
                        prefab->AddComponent(UEditor::Create(UEditor::AllComponents()[i]));
                    }
                }
                ImGui::EndCombo();
            }

            int i = 0;
            for (auto& [type, value] : prefab->Components())
            {
                value->ShowEditor(engine, true);
            }
        }

        ImGui::End();
    }
    void UEditorScene::ShowPrefabGallery()
    {
        const float thumbnailSize = 64.0f;
        const float padding = 12.0f;
        const float cellSize = thumbnailSize + padding;

        ImGui::BeginChild("EntityGallery", ImVec2(0, 0), true);

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = std::max(1, (int)(panelWidth / cellSize));

        ImGui::Columns(columns, nullptr, false);
        int i = 0;
        for (auto& [prefabName, value] : engine.Assets()->Prefabs())
        {
            ImGui::PushID(prefabName.c_str());

            if (value->HasComponent<CSprite>())
            {
                CSprite* sprite = value->GetComponent<CSprite>();
                auto& data = sprite->Data();
                
                ImTextureID id = (ImTextureID)(intptr_t)sprite->Sprite()->getTexture().getNativeHandle();
                sf::Vector2u size = sprite->Sprite()->getTexture().getSize();

                if (sprite->FlipX())
                {
                    ImVec2 uv0(
                        (float)(data.x + data.width) / size.x,
                        (float)data.y / size.y
                    );

                    ImVec2 uv1(
                        (float)data.x / size.x,
                        (float)(data.y + data.height) / size.y
                    );

                    ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);
                }
                else
                {
                    ImVec2 uv0(
                        (float)data.x / size.x,
                        (float)data.y / size.y);

                    ImVec2 uv1(
                        (float)(data.x + data.width) / size.x,
                        (float)(data.y + data.height) / size.y);

                    ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);
                }
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
                    prefab = value->Clone();
                    bShowEditPrefab = true;
                    bMainTab = false;
                }
                else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && bSceneLoaded)
                {
                    prefab = value->Clone();
                    bPrefabSelected = true;
                    bMainTab = false;
                }
            }

            ImGui::TextWrapped("%s", prefabName.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }
    }

    void UEditorScene::CreateScene()
    {
        std::string levelPath = "Assets/" + std::string(editorNameBuffer) + ".txt";
        std::ofstream file(levelPath);
        if (!file)
        {
            std::cerr << "Failed to create Assets/" << editorNameBuffer << ".txt" << std::endl;
            return;
        }

        file << LEVEL << " " << newSceneGridColumn << " " << newSceneGridRow << " " << newSceneGridSize << " " << "\n";
        file.close();
        LoadScene();
    }
    void UEditorScene::LoadScene()
    {
        Restart(editorNameBuffer);
        bSceneLoaded = true;
        bNewScene = false;
    }
    void UEditorScene::SaveScene()
    {
        std::string levelPath = "Assets/" + std::string(editorNameBuffer) + ".txt";
        if (std::filesystem::exists(levelPath))
        {
            std::string content;
            content.clear();

            content = LEVEL + " " + std::to_string(gridColumn) + " " + std::to_string(gridRow) + " " + std::to_string(gridSize) + "\n";

            /*for (const auto& [key, value] : prefabs)
            {
                content += PREFAB + " " + value->Name() + "\n";

                for (const auto& [c_key, c_value] : value->Components())
                {
                    content += COMPONENT + " " + c_value->ComponentName() + " " + c_value->Save() + "\n";
                }
            }*/

            for (auto& e : entities)
            {
                CTransform* transform = e->GetComponent<CTransform>();
                if (transform == nullptr) continue;
                content += ENTITY + " " + e->Name() + " " + std::to_string(transform->Position().x) + " " + std::to_string(transform->Position().y) + "\n";
            }

            std::ofstream out(levelPath);
            out << content;
            out.close();
        }
    }
    void UEditorScene::CloseScene()
    {
        ClearEditor();
        ClearScene();
        bSceneLoaded = false;
    }

    /*void UEditorScene::SaveAsset()
    {
        std::string assetsPath = "Assets/Assets.txt";
        if (std::filesystem::exists(assetsPath))
        {
            std::string content;
            content.clear();

            for (const auto& [key, value] : engine.Assets()->Sprites())
            {
                content += SPRITE + " " + key + " " + value.textureName + " " +
                    std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.width) + " " + std::to_string(value.height) + "\n";
            }

            auto& allAsstTypes = engine.Assets()->AllAssetTypes();
            if (allAsstTypes[assetTypeSelectedIndex] == SPRITE)
            {
                content += SPRITE + " " + newName + " " + engine.Assets()->AllTextures()[assetTextureSelectedIndex] + " " +
                    std::to_string(newSpriteX) + " " + std::to_string(newSpriteY) + " " + std::to_string(newSpriteWidth) + " " + std::to_string(newSpriteHeight) + "\n";
            }

            for (const auto& [key, value] : engine.Assets()->Animations())
            {
                content += ANIMATION + " " + key + " " + value.textureName + " " +
                    std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.width) + " " + std::to_string(value.height) + " " +
                    std::to_string(value.frame) + " " + std::to_string(value.speed) + "\n";
            }

            if (allAsstTypes[assetTypeSelectedIndex] == ANIMATION)
            {
                content += ANIMATION + " " + newName + " " + engine.Assets()->AllTextures()[assetTextureSelectedIndex] + " " +
                    std::to_string(newSpriteX) + " " + std::to_string(newSpriteY) + " " + std::to_string(newSpriteWidth) + " " + std::to_string(newSpriteHeight) + " " +
                    std::to_string(newAnimationFrame) + " " + std::to_string(newAnimationSpeed) + "\n";
            }
           
            std::ofstream out(assetsPath);
            out << content;
            out.close();
        }
    }*/

    //void UEditorScene::SaveUpdatePrefab()
    //{
    //    bShowEditPrefab = false;
    //    SaveScene();
    //    delete prefab;
    //    prefab = nullptr;
    //    bMainTab = true;
    //}
    /*void UEditorScene::SavePrefab()
    {
        prefabs.emplace(prefab->Name(), prefab->Clone());
        SaveScene();
        delete prefab;
        prefab = nullptr;
        bShowNewPrefab = false;
        bMainTab = true;
    }*/
    void UEditorScene::CancelPrefabSelected()
    {
        delete prefab;
        prefab = nullptr;
        bPrefabSelected = false;
        bMainTab = true;
    }

    void UEditorScene::ClearEditor()
    {
        editorNameBuffer[0] = '\0';
        assetTypeSelectedIndex = 0;
        assetTextureSelectedIndex = 0;
        newSpriteX = 0;
        newSpriteY = 0;
        newSpriteWidth = 0;
        newSpriteHeight = 0;
        newAnimationFrame = 0;
        newAnimationSpeed = 0;
        prefab = nullptr;
    }

    void UEditorScene::OnMouseRight()
    {
        if (bPrefabSelected)
        {
            bPrefabSelected = false;
            //delete prefab;
            prefab = nullptr;
            bMainTab = true;
        }
        else 
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(engine.RenderWindow());
            sf::Vector2f gridMousePosition = sf::Vector2f((mousePosition.x / gridSize) * gridSize,
                (mousePosition.y / gridSize) * gridSize);

            for (auto& e : entities)
            {
                CTransform* transform = e.get()->GetComponent<CTransform>();
                if (transform != nullptr)
                {
                    if (transform->Position().x == gridMousePosition.x &&
                        transform->Position().y == gridMousePosition.y)
                    {
                        e.get()->SetActive(false);
                        break;
                    }
                }
            }
        }
    }
    void UEditorScene::OnMouseLeft()
    {
        if (bPrefabSelected)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(engine.RenderWindow());
            
            CSprite* sprite = prefab->GetComponent<CSprite>();
            int x = ((mousePosition.x / gridSize) * gridSize) - (sprite->FlipX() ? -gridSize : 0);
            sf::Vector2f position = sf::Vector2f(x, (mousePosition.y / gridSize) * gridSize);

            AddEntity(prefab->Name(), position);
        }
    }

    
}


