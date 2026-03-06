#include "UGameScene.h"
#include <systems/SDrawSystem.h>
#include <systems/SStaticDrawSystem.h>

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
        AddSystem<SStaticDrawSystem>();
        AddSystem<SDrawSystem>(false);
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


