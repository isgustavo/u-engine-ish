#include "UEngine.h"
#include "UScene.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

namespace uei
{
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
    sf::Vector2f& UEngine::GridSize()
    {
        return gridSize;
    }
    void UEngine::Start()
    {
        currentScene = scenes.back().get();
        if (currentScene != nullptr)
            currentScene->Start();
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
            
            ImGui::Begin("u-engine-ish");
            ImGui::Checkbox("Show grid", &bShowGrid);
            ImGui::Checkbox("Show target", &bShowTarget);
            ImGui::End();

            if(currentScene != nullptr)
                currentScene->Update();

            renderWindow.clear();
            
            if (currentScene != nullptr)
                currentScene->Draw();

            if (bShowGrid)
                ShowGrid();

            ImGui::SFML::Render(renderWindow);
            renderWindow.display();
        }

        ImGui::SFML::Shutdown();
    }

    void UEngine::ShowGrid()
    {
        sf::Text gridText(font, "", 12);

        float leftX = renderWindow.getView().getCenter().x - (screenSize.x * 0.5f);
        float rightX = leftX + screenSize.x + gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)gridSize.x);

        for (float x = nextGridX; x < rightX; x += gridSize.x)
        {
            DrawLine(sf::Vector2f(x, 0), sf::Vector2f(x, screenSize.y));
        }

        for (float y = 0; y < screenSize.y; y += gridSize.y)
        {
            DrawLine(sf::Vector2f(leftX, screenSize.y - y), sf::Vector2f(rightX, screenSize.y - y));

            for (float x = nextGridX; x < rightX; x += gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)gridSize.x);
                int yTotalCell = (screenSize.y / gridSize.y) - 1;
                std::string yCell = std::to_string(yTotalCell - ((int)y / (int)gridSize.y));
                gridText.setString("(" + xCell + "," + yCell + ")");
                gridText.setPosition({ x + 3, screenSize.y - y - gridSize.y + 2 });
                renderWindow.draw(gridText);
            }
        }
    }

    void UEngine::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vertex line[] = { {p1, sf::Color::White}, {p2, sf::Color::White} };
        renderWindow.draw(line, 2, sf::PrimitiveType::Lines);
    }
}
