#include "TestScene.h"
#include "UEngine.h"
#include "components/CTransform.h"
#include "components/CSprite.h"
#include "components/CAnimation.h"

#include <iostream>

void TestScene::OnStart()
{
	std::cout << "OnStart" << std::endl;
	uei::UEntity& playerEntity = AddEntity(std::string("Player"));
	playerEntity.AddComponent<uei::CTransform>(uei::UFloat2D(0.f, 0.f));
	//const sf::Texture* texture = engine->Assets().GetTexture("Snake_leftRun");
	//if(texture != nullptr)
	playerEntity.AddComponent<uei::CSprite>(engine->Assets().GetTexture("Snake_leftRun"), 0, 0, 18, 18, sf::Vector2f(5.f, 5.f));

	playerEntity.AddComponent<uei::CAnimation>(engine->Assets().GetTexture("Snake_leftRun"), 7, 3, 21, 19, sf::Vector2f(5.f, 5.f));
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

	for (auto& e : entities)
	{
		auto animation = e.get()->GetComponent<uei::CAnimation>();
		if (animation == nullptr) continue;
		animation->Update();
		engine->RenderWindow().draw(animation->CurrentSprite());
	}
}
