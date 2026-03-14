#include "SMovementSystem.h"
#include <components/CTransform.h>
#include <components/CIdleAnimation.h>
#include <components/CMovement.h>

namespace uei
{
	SMovementSystem::SMovementSystem()
	{

	}

	void SMovementSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		for (auto& e : entities)
		{
			auto* cTransform = e->GetComponent<uei::CTransform>();
			auto* cMovement = e->GetComponent<uei::CMovement>();

			if (cTransform == nullptr || cMovement == nullptr) continue;

			EMovement movement = EMovement::NONE;

			if (cTransform->IsMoving())
			{
				if (cTransform->GetPosition().x > cTransform->GetPositionLastUpdate().x)
				{
					movement = EMovement::RIGHT;
					SetDiagonalMovement(cTransform, cMovement, movement);
				}
				else if (cTransform->GetPosition().x < cTransform->GetPositionLastUpdate().x)
				{
					movement = EMovement::LEFT;
					SetDiagonalMovement(cTransform, cMovement, movement);
				}
				else 
				{
					if (cTransform->GetPosition().y > cTransform->GetPositionLastUpdate().y)
					{
						movement = EMovement::UP;
					}
					else if (cTransform->GetPosition().y < cTransform->GetPositionLastUpdate().y)
					{
						movement = EMovement::DOWN;
					}
				}
			}

			cMovement->SetCurrentMovement(movement);
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