#include "UGameScene.h"
#include <systems/SDrawSystem.h>
#include <systems/SAnimationSystem.h>
#include <systems/SMovementSystem.h>

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
        bShowNavGrid = false;
        AddSystem<SMovementSystem>();
        AddSystem<SAnimationSystem>();
        AddDrawSystem<SDrawSystem>();
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


