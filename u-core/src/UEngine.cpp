#include "UEngine.h"
#include "UScene.h"
#include "editor/UEditorScene.h"
#include "UGameScene.h"
#include "editor/UEditor.h"
#include "components/CTransform.h"
#include "components/CPath.h"
#include "components/CSprite.h"
#include "components/CTarget.h"
#include "components/CAnimation.h"
#include "components/CIdleAnimation.h"
#include "components/CMovement.h"
#include "components/CMovementAnimation.h"
#include "components/CObstacle.h"
#include "components/CStaticDraw.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include<windows.h>
#include<algorithm>

namespace uei
{
    UEngine::UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit, const std::string& inStartLevelName) :
        screenSize(sf::Vector2f((float)inWidth, (float)inHeight)), 
        renderWindow(sf::VideoMode({ inWidth, inHeight }), windowName), 
        fixedDeltaTime(1000.f / framerateLimit),
        startLevelName(inStartLevelName)
    {
        //assets = std::make_unique<UAsset>();
        //assets->LoadFromFile(assetFilePath);
        assets = new UAsset();
        //renderWindow.setFramerateLimit(framerateLimit);
        ImGui::SFML::Init(renderWindow);

        //gridView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 512.0f, 512.0f}));
        //gridView = sf::View();// sf::FloatRect({ 0.0f, 0.0f }, { (float)inWidth, (float)inHeight }));
        //gridView.setSize({ (float)inWidth, (float)inHeight });
        //gridView.setCenter({ (float)inWidth * 0.5f, (float)inHeight * 0.5f });

        //float windowRatio = (float) inWidth / (float)inHeight;
        /*float viewRatio = 1.0f;

        float sizeX = 1.f;
        float sizeY = 1.f;
        float posX = 0.f;
        float posY = 0.f;

        if (windowRatio > viewRatio)
        {
            sizeX = viewRatio / windowRatio;
            posX = (1.f - sizeX) * 0.5f;
        }
        else
        {
            sizeY = windowRatio / viewRatio;
            posY = (1.f - sizeY) * 0.5f;
        }

        std::cout << sizeX << "," << sizeY << std::endl;
        std::cout << posX << "," << posY << std::endl;*/

        //gridView.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 0.1f, 1.0f }));

        //renderWindow.setView(gridView);
        //bShowGrid = true;
        //bShowNewPrefab = false;
        //font = sf::Font(fontPath); 
    }

    UEngine::~UEngine()
    {
        delete assets;
        assets = nullptr;

        for (auto* s : scenes)
        {
            s->ClearScene();
            delete s;
            s = nullptr;
        }
        scenes.clear();
        currentScene = nullptr;
    }

    sf::Vector2f& UEngine::ScreenSize()
    {
        return screenSize;
    }
    void UEngine::Start()
    {
        assets->Load(*this);
        scenes.clear();
        if(startLevelName == EMPTY)
            currentScene = AddScene<UEditorScene>(startLevelName);
        else 
            currentScene = AddScene<UGameScene>(startLevelName);
    }
  
    void UEngine::Update()
    {
        sf::Clock clock;
        float startTime = 0.0f;
        while (renderWindow.isOpen())
        {
            sf::Time time = clock.restart();
            float startTime = time.asSeconds();
            while (const auto event = renderWindow.pollEvent())
            {
                ImGui::SFML::ProcessEvent(renderWindow, *event);

                if (event->is<sf::Event::Closed>())
                {
                    renderWindow.close();
                }

                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    if (!ImGui::GetIO().WantCaptureMouse && currentScene != nullptr)
                    {
                        const auto& mouse = event->getIf<sf::Event::MouseButtonPressed>();

                        if (mouse->button == sf::Mouse::Button::Left)
                        {
                            currentScene->OnMouseLeft();
                        }
                        if (mouse->button == sf::Mouse::Button::Right)
                        {
                            currentScene->OnMouseRight();
                        }
                    }
                }
            }

            ImGui::SFML::Update(renderWindow, time);
            
            if (currentScene != nullptr && !currentScene->bIsStarted)
                currentScene->Start();

            if(currentScene != nullptr)
                currentScene->Update(fixedDeltaTime);

            //if (currentScene != nullptr)
            //{
            //    while (deltaTimeAccumulator >= fixedDeltaTime)
            //    {
            //        currentScene->Update(fixedDeltaTime);
            //    }
            //}

            renderWindow.clear();
            
            if (currentScene != nullptr)
                currentScene->Draw();

            ImGui::SFML::Render(renderWindow);
            renderWindow.display();

            time = clock.restart();
            float elapsed = time.asSeconds() - startTime;
            Sleep(std::max(0.f, fixedDeltaTime - elapsed));
        }

        ImGui::SFML::Shutdown();
    }

    void UEngine::AddGameScene(std::string levelName)
    {
        currentScene = AddScene<UGameScene>(levelName);
    }

    void UEngine::Clear()
    {
        scenes.clear();
        delete currentScene;
        currentScene = nullptr;
    }
}

//REGISTER_COMPONENT(CTarget);
//REGISTER_COMPONENT(CAnimation);
REGISTER_COMPONENT(CObstacle);
REGISTER_COMPONENT(CStaticDraw);
REGISTER_COMPONENT(CIdleAnimation);
REGISTER_COMPONENT(CMovement);
REGISTER_COMPONENT(CMovementAnimation);
REGISTER_COMPONENT(CSprite);
REGISTER_COMPONENT(CTransform);