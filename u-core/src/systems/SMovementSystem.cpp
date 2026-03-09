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

			cMovement->SetMovement(EMovement::NONE);

			if (cTransform->IsMoving())
			{
				if (cTransform->Position().x > cTransform->PositionLastUpdate().x)
				{
					cMovement->SetMovement(EMovement::RIGHT);
					SetDiagonalMovement(cTransform, cMovement);
				}
				else if (cTransform->Position().x < cTransform->PositionLastUpdate().x)
				{
					cMovement->SetMovement(EMovement::LEFT);
					SetDiagonalMovement(cTransform, cMovement);
				}
				else 
				{
					if (cTransform->Position().y > cTransform->PositionLastUpdate().y)
					{
						cMovement->SetMovement(EMovement::UP);
					}
					else if (cTransform->Position().y < cTransform->PositionLastUpdate().y)
					{
						cMovement->SetMovement(EMovement::DOWN);
					}
				}
			}
		}
	}

	void SMovementSystem::SetDiagonalMovement(CTransform* transform, CMovement* movement)
	{
		if (movement->GridMovement() == EGrid::GRID_8)
		{
			if (transform->Position().y > transform->PositionLastUpdate().y)
			{
				movement->SetMovement(movement->GetMovement() == EMovement::RIGHT ? EMovement::UP_RIGHT : EMovement::UP_LEFT);
			}
			else if (transform->Position().y < transform->PositionLastUpdate().y)
			{
				movement->SetMovement(movement->GetMovement() == EMovement::RIGHT ? EMovement::DOWN_RIGHT : EMovement::DOWN_LEFT);
			}
		}
	}
}