#include "UGameScene.h"
#include <systems/SDrawSystem.h>
#include <systems/SStaticDrawSystem.h>
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
        AddSystem<SMovementSystem>();
        AddSystem<SAnimationSystem>();
        AddDrawSystem<SStaticDrawSystem>();
        AddDrawSystem<SDrawSystem>(false);
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


