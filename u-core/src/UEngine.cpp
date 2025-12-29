#include "UEngine.h"
#include "UScene.h"
#include "math/UFloat2D.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <string>

namespace uei
{
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
            ImGui::End();

            if(currentScene != nullptr)
                currentScene->Update();

            renderWindow.clear();
            
            TryShowGrid();
            
            if (currentScene != nullptr)
                currentScene->Draw();

            ImGui::SFML::Render(renderWindow);
            renderWindow.display();
        }

        ImGui::SFML::Shutdown();
    }

    void UEngine::TryShowGrid()
    {
        if (bShowGrid)
            ShowGrid();
    }

    void UEngine::ShowGrid()
    {
        sf::Text gridText(font, "", 12);

        float leftX = renderWindow.getView().getCenter().x - (width * 0.5f);
        float rightX = leftX + width + gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)gridSize.x);

        for (float x = nextGridX; x < rightX; x += gridSize.x)
        {
            DrawLine(uei::UFloat2D(x, 0), uei::UFloat2D(x, height));
        }

        for (float y = 0; y < height; y += gridSize.y)
        {
            DrawLine(uei::UFloat2D(leftX, height - y), uei::UFloat2D(rightX, height - y));

            for (float x = nextGridX; x < rightX; x += gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)gridSize.x);
                std::string yCell = std::to_string((int)y / (int)gridSize.y);
                gridText.setString("(" + xCell + "," + yCell + ")");
                gridText.setPosition({ x + 3, height - y - gridSize.y + 2 });
                renderWindow.draw(gridText);
            }
        }
    }

    void UEngine::DrawLine(const uei::UFloat2D& p1, const uei::UFloat2D& p2)
    {
        sf::Vertex line[] = { {{p1.x, p1.y}, sf::Color::White}, {{p2.x, p2.y}, sf::Color::White} };
        renderWindow.draw(line, 2, sf::PrimitiveType::Lines);
    }
}
