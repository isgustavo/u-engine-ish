#include "UGameScene.h"
#include <systems/SDrawSystem.h>
#include <systems/SAnimationSystem.h>
#include <systems/SMovementSystem.h>
#include <systems/SPathfinderSystem.h>
#include <systems/SPathFollowSystem.h>

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
    }

    void UGameScene::OnUpdate(float deltaTime)
    {

    }

    void UGameScene::OnDraw()
    {
        DrawGrid();
    }

    void UGameScene::OnMouseLeft()
    {

    }

    void UGameScene::OnMouseRight()
    {

    }
}


