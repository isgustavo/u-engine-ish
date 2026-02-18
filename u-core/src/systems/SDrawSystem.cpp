#include "SDrawSystem.h"
#include "components/CTransform.h"
#include "components/CSprite.h"

namespace uei
{
	SDrawSystem::SDrawSystem()
	{
	}

	void SDrawSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		for (auto& e : entities)
		{
			auto* c_transform = e->GetComponent<uei::CTransform>();
			auto* c_sprite = e->GetComponent<uei::CSprite>();

			if (c_transform == nullptr || c_sprite == nullptr) continue;

			sf::Sprite* sp = c_sprite->Sprite();
			sp->setPosition(c_transform->Position());

			engine.RenderWindow().draw(*sp);
		}
	}
}