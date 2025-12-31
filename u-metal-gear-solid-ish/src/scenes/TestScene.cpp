#include "TestScene.h"
#include "UEngine.h"
#include "components/CTransform.h"
#include "components/CSprite.h"

#include <iostream>

void TestScene::OnStart()
{
	std::cout << "OnStart" << std::endl;
	uei::UEntity& playerEntity = AddEntity(std::string("Player"));
	playerEntity.AddComponent<uei::CTransform>(uei::UFloat2D(64.f, 64.f));
	//const sf::Texture* texture = engine->Assets().GetTexture("Snake_leftRun");
	//if(texture != nullptr)
	playerEntity.AddComponent<uei::CSprite>(engine->Assets().GetTexture("Snake_leftRun"));
}

void TestScene::OnUpdate()
{
	std::cout << "OnUpdate" << std::endl;
}

void TestScene::OnDraw()
{
	for (auto& e : entities)
	{
		engine->RenderWindow().draw(e.get()->GetComponent<uei::CSprite>()->GetSprite());
	}
}
