#include "UEngine.h"
#include "UScene.h"
#include "editor/UEditorScene.h"
#include "editor/UEditor.h"
#include "components/CTransform.h"
#include "components/CPath.h"
#include "components/CSprite.h"
#include "components/CTarget.h"
#include "components/CAnimation.h"

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


namespace uei
{
    UEngine::UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit, bool showEditor) :
        screenSize(sf::Vector2f((float)inWidth, (float)inHeight)), 
        renderWindow(sf::VideoMode({ inWidth, inHeight }), windowName), 
        bShowEditor(showEditor)
    {
        //assets = std::make_unique<UAsset>();
        //assets->LoadFromFile(assetFilePath);
        assets = new UAsset();

        renderWindow.setFramerateLimit(framerateLimit);
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
            s->Clear();
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
        if (bShowEditor)
        {
            scenes.clear();
            currentScene = AddScene<uei::UEditorScene>();
            currentScene->Start(" ");
        }
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

            ImGui::SFML::Update(renderWindow, deltaClock.restart());
            
            if(currentScene != nullptr)
                currentScene->Update();

            renderWindow.clear();
            
            if (currentScene != nullptr)
                currentScene->Draw();

            ImGui::SFML::Render(renderWindow);
            renderWindow.display();
        }

        ImGui::SFML::Shutdown();
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
REGISTER_COMPONENT(CSprite);
REGISTER_COMPONENT(CTransform);