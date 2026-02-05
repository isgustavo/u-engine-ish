#include "UEditorScene.h"
#include "editor/Editor_ComponentRegistry.h"

#include <SFML/Graphics/RectangleShape.hpp>


namespace uei
{
    UEditorScene::UEditorScene(UEngine& inEngine) : UScene(inEngine)
    {

    }
    void UEditorScene::OnStart()
    {
        view = sf::View(sf::FloatRect({ 0.f, 0.f }, { 512.0f, 512.0f}));
        view = sf::View();// sf::FloatRect({ 0.0f, 0.0f }, { (float)inWidth, (float)inHeight }));
        view.setSize({ 512.0f, 512.0f });
        view.setCenter({ 512.0f * 0.5f, 512.0f * 0.5f });

        std::cout << "OnStart" << std::endl;

        //for (auto& [k, v] : uei::Editor_ComponentRegistry::All())
        //{
        //    std::cout << k << std::endl;
        //}
    }

    void UEditorScene::OnUpdate()
    {
        //std::cout << "OnUpdate" << std::endl;
    }

    void UEditorScene::OnDraw()
    {
        engine.RenderWindow().setView(view);
        sf::RectangleShape rect = sf::RectangleShape({ 90.0f, 90.f });
        rect.setFillColor(sf::Color::White);
        engine.RenderWindow().draw(rect);
    }

    void UEditorScene::Editor_Show() { }
}


