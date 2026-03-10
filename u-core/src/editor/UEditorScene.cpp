#include "UEditorScene.h"
#include "UEditor.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics.hpp>
#include <components/CTransform.h>
#include <components/CSprite.h>
#include <components/CIdleAnimation.h>
#include <systems/SDrawSystem.h>

#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include <systems/SStaticDrawSystem.h>
#include <systems/SAnimationSystem.h>


namespace uei
{
    UEditorScene::UEditorScene(UEngine& inEngine, std::string levelName) : UScene(inEngine, levelName) { }

    UEditorScene::~UEditorScene()
    {
        delete prefab;
        prefab = nullptr;
    }

    void UEditorScene::OnStart()
    {
        bMainTab = true;
        bShowNavGrid = true;
        AddSystem<SAnimationSystem>();
        AddDrawSystem<SDrawSystem>(true);
    }

    void UEditorScene::OnUpdate(float deltaTime)
    {
        currentAnimationDeltaTime += deltaTime;

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
            CTransform* cTransform = prefab->GetComponent<CTransform>();
            CSprite* cSprite = prefab->GetComponent<CSprite>();
            
            sf::Vector2i mousePosition = sf::Mouse::getPosition(engine.RenderWindow());
            int x = ((mousePosition.x / gridSize) * gridSize) + (cSprite->FlipX() ? gridSize : 0);
            int y = ((mousePosition.y / gridSize) * gridSize) + (cSprite->FlipY() ? gridSize : 0);

            cTransform->SetPosition(sf::Vector2f(x, y));
        }
    }

    void UEditorScene::OnDraw()
    {
        if (bSceneLoaded)
            DrawGrid();

        engine.RenderWindow().setView(view);

        if (bPrefabSelected)
        {
            CTransform* cTransform = prefab->GetComponent<CTransform>();
            CIdleAnimation* cIdleAnimation = prefab->GetComponent<CIdleAnimation>();
            CSprite* cSprite = prefab->GetComponent<CSprite>();

            std::string textureName;
            int x, y, width, height;
            bool flipX = false, flipY = false;
            if (cIdleAnimation != nullptr)
            {
                if (cIdleAnimation->GetAnimationAsset() != nullptr)
                {
                    const AnimationAsset* animationAsset = cIdleAnimation->GetAnimationAsset();// engine.Assets()->GetAnimation(cIdleAnimation->GetIdleAnimationName());
                    textureName = animationAsset->TextureName;
                    int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset->Speed) % animationAsset->Frame;
                    x = animationAsset->X + (currentAnimationFrame * animationAsset->Width);
                    y = animationAsset->Y;
                    width = animationAsset->Width;
                    height = animationAsset->Height;
                }
            }
            else
            {
                if (cSprite->GetSpriteAsset() != nullptr)
                {
                    const SpriteAsset* spriteAsset = cSprite->GetSpriteAsset();// engine.Assets()->GetSpriteAsset(cSprite->SpriteName());
                    textureName = spriteAsset->TextureName;
                    x = spriteAsset->X;
                    y = spriteAsset->Y;
                    width = spriteAsset->Width;
                    height = spriteAsset->Height; 
                    flipX = cSprite->FlipX();
                    flipY = cSprite->FlipY();
                }
            }

            sf::Sprite sprite = sf::Sprite(engine.Assets()->GetTexture(textureName), sf::IntRect({ x, y }, { width, height }));
            sprite.setScale(sf::Vector2f((flipX ? -1 : 1) * (gridSize / sprite.getTextureRect().size.x),
                (flipY ? -1 : 1) * gridSize / sprite.getTextureRect().size.y));

            sprite.setPosition(cTransform->Position());

            engine.RenderWindow().draw(sprite);
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
            SaveScene();
            engine.AddGameScene(levelNameLoaded);
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
                    AddImGui(allTexture[assetTextureSelectedIndex], newSpriteX, newSpriteY, newSpriteWidth, newSpriteHeight, false, false);
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
                    ImGui::InputFloat("##AddNewSpeed", &newAnimationSpeed, 0);
                    ImGui::PopItemWidth();

                    if (newAnimationFrame > 0 && newAnimationSpeed > 0)
                    {
                        AddImGui(allTexture[assetTextureSelectedIndex], newSpriteX, newSpriteY, newSpriteWidth, newSpriteHeight,
                            newAnimationSpeed, newAnimationFrame);
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
        for (auto& [key, value] : engine.Assets()->AllSpriteAssets())
        {
            ImGui::PushID(key.c_str());

            AddImGui(&engine.Assets()->GetSpriteAsset(value.AssetName), false, false, thumbnailSize);

            ImGui::TextWrapped("%s", value.AssetName.c_str());
            if (ImGui::Button("Remove"))
            {
                //onRemove();
            }
            ImGui::NextColumn();
            ImGui::PopID();
        }
        for (auto& [key, value] : engine.Assets()->Animations())
        {
            ImGui::PushID(key.c_str());

            AddImGui(&engine.Assets()->GetAnimation(value.AssetName), thumbnailSize);

            ImGui::TextWrapped("%s", value.AssetName.c_str());
            if (ImGui::Button("Remove"))
            {
                //onRemove();
            }
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
                prefab->AddComponent<CTransform>();
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
                engine.Assets()->AddPrefab(editorNameBuffer, prefab->Clone());
                engine.Assets()->Save();
                ClearEditor();
                bMainTab = true;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            bShowNewPrefab = false;
            ClearEditor();
            bMainTab = true;
        }

        if (bShowNewPrefab)
        {
            ShowPrefabComponent();
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
            engine.Assets()->Save();
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
        
        if(bShowEditPrefab)
        {
            ShowPrefabComponent();
        }

        ImGui::End();
    }
    void UEditorScene::ShowPrefabComponent()
    {
        std::vector<std::type_index> prefabComponentToRemove;

        int selectedIndex = 0;
        ImGui::Text("Add Components:");
        ImGui::SameLine();
        if (ImGui::BeginCombo("##AddComponents", " "))
        {
            for (int i = 0; i < UEditor::AllComponents().size(); i++)
            {
                const bool isSelected = (selectedIndex == i);

                //if (UEditor::AllComponents()[i] == "CTransform") continue;

                bool hasComponent = false;
                for (auto& [key, value] : prefab->Components())
                {
                    if (value->ComponentName() == UEditor::AllComponents()[i])
                    {
                        hasComponent = true;
                        break;
                    }
                }

                if (hasComponent) continue;

                if (ImGui::Selectable(UEditor::AllComponents()[i].c_str(), isSelected))
                {
                    prefab->AddComponent(UEditor::Create(UEditor::AllComponents()[i]), true);
                }
            }
            ImGui::EndCombo();
        }

        for (auto& [type, value] : prefab->Components())
        {
            value->ShowEditor(engine, [&prefabComponentToRemove, type]()
                {
                    prefabComponentToRemove.push_back(type);
                }
            );
        }

        for (int i = 0; i < prefabComponentToRemove.size(); i++)
        {
            prefab->RemoveComponent(prefabComponentToRemove[i]);
        }
    }
    void UEditorScene::ShowPrefabGallery()
    {
        const float thumbnailSize = 64.0f;
        const float padding = 12.0f;
        const float cellSize = thumbnailSize + padding;

        ImGui::BeginChild("EntityGallery", ImVec2(0, 0), true);

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columns = std::max(1, (int)(panelWidth / cellSize));

        std::vector<std::string> prefabToRemove;

        ImGui::Columns(columns, nullptr, false);
        int prefabIndex = 0;
        for (auto& [prefabName, value] : engine.Assets()->Prefabs())
        {
            ImGui::PushID(prefabName.c_str());
            bool bShowDefaultThumb = true;

            CIdleAnimation* cIdleAnimation = value->GetComponent<CIdleAnimation>();
            CSprite* cSprite = value->GetComponent<CSprite>();
            if (cIdleAnimation != nullptr)
            {
                if (cIdleAnimation->GetAnimationAsset() != nullptr)
                {
                    AddImGui(cIdleAnimation->GetAnimationAsset());
                    bShowDefaultThumb = false;
                }
            }
            else if (cSprite != nullptr)
            {
                if (cSprite->GetSpriteAsset() != nullptr)
                {
                    AddImGui(cSprite->GetSpriteAsset(), cSprite->FlipX(), cSprite->FlipY());
                    bShowDefaultThumb = false;
                }
            }

            if (bShowDefaultThumb)
            {
                if (ImGui::Button("##thumb" + (++prefabIndex), ImVec2(thumbnailSize, thumbnailSize)))
                {

                }
            }

            if (ImGui::IsItemHovered())
            {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && bSceneLoaded)
                {
                    prefab = value;// ->Clone();
                    bPrefabSelected = true;
                    bMainTab = false;
                }
            }

            ImGui::TextWrapped("%s", prefabName.c_str());
            if (ImGui::Button("Edit"))
            {
                prefab = value;// ->Clone();
                bShowEditPrefab = true;
                bMainTab = false;
            }
            if (ImGui::Button("Remove"))
            {
                prefabToRemove.push_back(prefabName);
            }
            ImGui::NextColumn();
            ImGui::PopID();
        }

        for (auto prefabName : prefabToRemove)
        {
            engine.Assets()->RemovePrefab(prefabName);
        }

        if (prefabToRemove.size() > 0)
        {
            engine.Assets()->Save();
        }
    }

    void UEditorScene::AddImGui(const AnimationAsset* animationData, float thumbnailSize)
    {
        AddImGui(animationData->TextureName, animationData->X, animationData->Y, animationData->Width, animationData->Height,
            animationData->Speed, animationData->Frame, thumbnailSize);
    }
    void UEditorScene::AddImGui(const std::string& textureName, int x, int y, int width, int height, float speed, int frame, float thumbnailSize)
    {
        int currentAnimationFrame = (int)(currentAnimationDeltaTime * speed) % frame;
        int spriteX = x + (currentAnimationFrame * width);

        sf::Sprite sprite = sf::Sprite(engine.Assets()->GetTexture(textureName), sf::IntRect({ spriteX, y }, { width, height }));

        ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
        sf::Vector2u size = sprite.getTexture().getSize();

        ImVec2 uv0((float)spriteX / size.x, (float)y / size.y);
        ImVec2 uv1((float)(spriteX + width) / size.x, (float)(y + height) / size.y);

        ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);
    }
    void UEditorScene::AddImGui(const SpriteAsset* spriteData, bool flipX, bool flipY, float thumbnailSize)
    {
        AddImGui(spriteData->TextureName, spriteData->X, spriteData->Y, spriteData->Width, spriteData->Height, flipX, flipY);
    }
    void UEditorScene::AddImGui(const std::string& textureName, int x, int y, int width, int height, bool flipX, bool flipY, float thumbnailSize)
    {
        sf::Sprite sprite = sf::Sprite(engine.Assets()->GetTexture(textureName), sf::IntRect({ x, y }, { width, height }));

        ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
        sf::Vector2u size = sprite.getTexture().getSize();

        ImVec2 uv0(
            flipX ? (float)(x + width) / size.x : (float)x / size.x,
            flipY ? (float)(y + height) / size.y : (float)y / size.y
        );

        ImVec2 uv1(
            flipX ? (float)x / size.x : (float)(x + width) / size.x,
            flipY ? (float)y / size.y : (float)(y + height) / size.y
        );

        ImGui::Image(id, ImVec2(thumbnailSize, thumbnailSize), uv0, uv1);
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
        std::string levelPath = "Assets/" + std::string(editorNameBuffer) + ".txt";
        if (!std::filesystem::exists(levelPath))
        {
            bSceneLoaded = false;
            bNewScene = true;
            return;
        }
        levelNameLoaded = editorNameBuffer;
        Restart(levelNameLoaded);
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
            
            int x = (mousePosition.x / gridSize) * gridSize;
            int y = (mousePosition.y / gridSize) * gridSize;

            AddEntity(prefab->Clone(), sf::Vector2f(x, y));
        }
        //else 
        //{
        //    sf::Vector2i mousePosition = sf::Mouse::getPosition(engine.RenderWindow());
        //    int x = (mousePosition.x / gridSize) * gridSize;
        //    int y = (mousePosition.y / gridSize) * gridSize;

        //    int eColumns = (int)x / gridSize;
        //    int eRows = (int)y / gridSize;
        //}
    }
}


