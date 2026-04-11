#include "UGameScene.h"

namespace uei 
{
    UGameScene::UGameScene(UEngine& inEngine, std::string levelName) : UScene(inEngine, levelName)
    {

    }

    UGameScene::~UGameScene()
    {
    }

    void UGameScene::OnStart()
    {
        bShowNavGrid = true;
    }

    void UGameScene::OnInput(const std::optional<class sf::Event> event)
    {
        for (auto& [inputIndex, lable] : inputKeys)
        {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(inputIndex);
            currentPlayerInput.SetInput(key, sf::Keyboard::isKeyPressed(key));
        }
    }

    void UGameScene::OnUpdate(float deltaTime)
    {

    }

    void UGameScene::OnDraw()
    {

    }

    void UGameScene::OnMouseLeft()
    {

    }

    void UGameScene::OnMouseRight()
    {

    }
}


