#include "TestScene.h"
#include "UEngine.h"
#include "components/CTransform.h"
#include "components/CSprite.h"
#include "components/CAnimation.h"

#include <iostream>
#include <components/CRect.h>
#include <components/CTarget.h>
#include <components/CAgent.h>
#include <components/CSpeed.h>
#include <components/CObstacle.h>
#include <components/CFindPathTo.h>
#include <components/CPathRequest.h>
#include <components/CNavGridModifier.h>
#include <components/CPath.h>
#include <systems/SPathfinderSystem.h>


TestScene::TestScene(uei::UEngine& inEngine) :
	uei::UScene(inEngine)
{
	std::cout << "TestScene" << std::endl;
}

void TestScene::OnStart()
{
	std::cout << "OnStart" << std::endl;
	uei::UEntity& playerEntity = AddEntity(std::string("Player"));
	playerEntity.AddComponent<uei::CTransform>(sf::Vector2f(0.f, 90.f));
	//playerEntity.AddComponent<uei::CSprite>(engine->Assets().GetTexture("Snake_leftRun"), 0, 0, 18, 18, sf::Vector2f(5.f, 5.f));
	playerEntity.AddComponent<uei::CAnimation>(engine.Assets().GetTexture("Snake_leftRun"), 7, 3, 21, 19, sf::Vector2f(5.f, 5.f));
	playerEntity.AddComponent<uei::CSpeed>(1.0f);
	
	for (size_t i = 0; i < engine.Rows(); i++)
	{
		for (size_t j = 0; j < engine.Columns(); j++)
		{
			uei::UEntity& newEntity = AddEntity(std::string("Tile"));
			newEntity.AddComponent<uei::CTransform>(sf::Vector2f
			(
				(i * engine.GridSize().x),
				(j * engine.GridSize().y)
			));

			if ((i >= 4 && i <= 6) && (j >= 4 && j <= 6))
			{
				newEntity.AddComponent<uei::CNavGridModifier>(1, 1, -1);
				newEntity.AddComponent<uei::CRect>(engine.GridSize(), sf::Color(29.f, 36.f, 41.f));
			}
			else 
			{
				newEntity.AddComponent<uei::CNavGridModifier>(1, 1, 0);
				newEntity.AddComponent<uei::CRect>(engine.GridSize(), sf::Color(25.f, 25.f, 56.f));
			}
		}
	}

	uei::UEntity& target = AddEntity(std::string("Target"));
	target.AddComponent<uei::CTransform>(sf::Vector2f(
		(8.f * engine.GridSize().x),
		(8.f * engine.GridSize().y)
	));
	target.AddComponent<uei::CRect>(engine.GridSize(), sf::Color::Red);
	target.AddComponent<uei::CTarget>();

	uei::UEntity& agent = AddEntity(std::string("Agent"));
	agent.AddComponent<uei::CTransform>(sf::Vector2f(
		(2.f * engine.GridSize().x),
		(2.f * engine.GridSize().y)
	));
	agent.AddComponent<uei::CAgent>(engine.GridSize());
	agent.AddComponent<uei::CPathRequest>(sf::Vector2f(
		(8.f * engine.GridSize().x),
		(8.f * engine.GridSize().y)
	));

	SetNavGridDirty();
	AddSystem<uei::SPathfinderSystem>(uei::Heuristic::ManhattanOctagonal, navGrid, navGridColumnSize, navGridSqrSize);
}
void TestScene::OnUpdate()
{
	//std::cout << "OnUpdate" << std::endl;

	for (auto& e : entities)
	{
		auto c_transform = e.get()->GetComponent<uei::CTransform>();
		auto c_rect = e.get()->GetComponent<uei::CRect>();

		if (c_transform != nullptr && c_rect != nullptr)
		{
			if (c_transform->ShouldUpdate())
			{
				c_rect->Rect().setPosition(c_transform->Position());
			}
		}

		auto c_speed = e.get()->GetComponent<uei::CSpeed>();
		auto c_animation = e.get()->GetComponent<uei::CAnimation>();
		if (c_transform != nullptr && c_animation != nullptr && c_speed != nullptr)
		{
			c_transform->SetPosition(c_transform->Position() + (sf::Vector2f(1.0f, 0.0f) * c_speed->Speed()));
		}
	}

	for (auto& e : entities)
	{
		auto c_agent = e.get()->GetComponent<uei::CAgent>();
		auto c_pathRequest = e.get()->GetComponent<uei::CPathRequest>();
		if (c_pathRequest == nullptr) continue;
	}

	for (auto& s : systems)
	{
		s.get()->Update(entities);
	}
}

void TestScene::OnDraw()
{
	for (auto& e : entities)
	{
		auto c_transform = e.get()->GetComponent<uei::CTransform>();
		auto c_target = e.get()->GetComponent<uei::CTarget>();
		auto c_rect = e.get()->GetComponent<uei::CRect>();
		if (c_transform != nullptr && c_rect != nullptr && c_target == nullptr)
		{
			engine.RenderWindow().draw(c_rect->Rect());
		}
	}
	
	for (auto& e : entities)
	{
		auto c_transform = e.get()->GetComponent<uei::CTransform>();
		auto c_sprite = e.get()->GetComponent<uei::CSprite>();
		if (c_sprite != nullptr && c_transform != nullptr)
		{
			auto sprite = c_sprite->Sprite();
			sprite.setPosition(c_transform->Position());
			engine.RenderWindow().draw(sprite);
		}
	}

	for (auto& e : entities)
	{
		auto c_transform = e.get()->GetComponent<uei::CTransform>();
		auto c_animation = e.get()->GetComponent<uei::CAnimation>();
		if (c_animation != nullptr && c_transform != nullptr)
		{
			c_animation->Update();
			auto sprite = c_animation->CurrentSprite();
			sprite.setPosition(c_transform->Position());
			engine.RenderWindow().draw(sprite);
		}
	}

	if (engine.ShouldShowGrid())
	{
		for (auto& e : entities)
		{
			auto c_transform = e.get()->GetComponent<uei::CTransform>();
			auto c_rect = e.get()->GetComponent<uei::CRect>();
			auto c_target = e.get()->GetComponent<uei::CTarget>();
			if (c_target != nullptr && c_transform != nullptr && c_rect != nullptr)
			{
				auto sprite = c_rect->Rect();
				sprite.setPosition(c_transform->Position());
				engine.RenderWindow().draw(sprite);
			}
		}

		for (auto& e : entities)
		{
			auto c_transform = e.get()->GetComponent<uei::CTransform>();
			auto c_target = e.get()->GetComponent<uei::CAgent>();
			if (c_target != nullptr && c_transform != nullptr)
			{
				auto sprite = c_target->Rect();
				sprite.setPosition(c_transform->Position());
				engine.RenderWindow().draw(sprite);
			}

			auto c_path = e.get()->GetComponent<uei::CPath>();

			if (c_path != nullptr)
			{
				for (auto& p : c_path->Path())
				{
					auto rect = sf::RectangleShape(engine.GridSize() * .25f);
					rect.setPosition(sf::Vector2f((p.x * engine.GridSize().x) + engine.GridSize().x * .37f, 
						p.y * engine.GridSize().y + engine.GridSize().y * .37f));
					rect.setFillColor(sf::Color::Yellow);
					engine.RenderWindow().draw(rect);
				}
			}
		}
	}
}
