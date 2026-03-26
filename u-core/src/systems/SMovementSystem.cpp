#include "SMovementSystem.h"
#include <components/CTransform.h>
#include <components/CMovement.h>

namespace uei
{
	SMovementSystem::SMovementSystem()
	{

	}

	void SMovementSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		for (auto& e : entities)
		{
			auto* cTransform = e->GetComponent<CTransform>();
			auto* cMovement = e->GetComponent<CMovement>();

			if (cTransform == nullptr || cMovement == nullptr) continue;
			
			sf::Vector2i movement = MovementToVector(cMovement->GetCurrentMovement());
			cTransform->SetPosition(sf::Vector2f(cTransform->GetPosition() + sf::Vector2f(movement.x * 0.25f * engine.DeltaTime(), movement.y * 0.25f * engine.DeltaTime())));
		}
	}

	void SMovementSystem::SetDiagonalMovement(CTransform* ctransform, CMovement* cMovement, EMovement& movement)
	{
		if (cMovement->GridMovement() == EGrid::GRID_8)
		{
			if (ctransform->GetPosition().y > ctransform->GetPositionLastUpdate().y)
			{
				movement = (movement == EMovement::RIGHT) ? EMovement::UP_RIGHT : EMovement::UP_LEFT;
			}
			else if (ctransform->GetPosition().y < ctransform->GetPositionLastUpdate().y)
			{
				movement = (movement == EMovement::RIGHT) ? EMovement::DOWN_RIGHT : EMovement::DOWN_LEFT;
			}
		}
	}
}