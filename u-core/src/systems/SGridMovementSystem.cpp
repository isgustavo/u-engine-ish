#include "SGridMovementSystem.h"
#include <UEngine.h>
#include <components/CTransform.h>
#include <components/CGridMovement.h>
#include <components/CPlayer.h>

#include <SFML/Graphics/RectangleShape.hpp>

namespace uei
{
	SGridMovementSystem::SGridMovementSystem()
	{

	}

	void SGridMovementSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		for (auto& e : entities)
		{
			auto* cTransform = e->GetComponent<CTransform>();
			auto* cGridMovement = e->GetComponent<CGridMovement>();

			if (cTransform == nullptr || cGridMovement == nullptr) continue;

			//if (cGridMovement->GetCurrentMovement() == EMovement::NONE) continue;
			if (cGridMovement->IsStop()) continue;

			float lerpTime = cGridMovement->GetLerpTime();
			if (lerpTime > 1.0f)
			{
				cGridMovement->SetStop(true);
				continue;

				/*lerpTime = 0.0f;
				cMovement->SetGridMovementLerpTime(lerpTime);
				sf::Vector2i movement = MovementToVector(cMovement->GetCurrentMovement());
				sf::Vector2i currentPosition = sf::Vector2i(cTransform->GetPosition().x, cTransform->GetPosition().y);
				sf::Vector2i targetMovement = sf::Vector2i(currentPosition.x + (movement.x * engine.CurrentScene()->GridSize()), currentPosition.y + (movement.y * engine.CurrentScene()->GridSize()));
				cMovement->SetGridTargetMovement(currentPosition, targetMovement);*/
			}

			cGridMovement->UpdateLerpTime(5 * engine.DeltaTime());
			sf::Vector2i newGridPosition = LerpMovement(cGridMovement->GetGridStartMovement(), cGridMovement->GetGridTargetMovement(), cGridMovement->GetLerpTime());
			
			//sf::Vector2i t = GridToPosition(newGridPosition, engine.CurrentScene()->GridSize());
			//std::cout << t.x << "," << t.y << "(" << newGridPosition.x << "," << newGridPosition.y << std::endl;
			cTransform->SetPosition(newGridPosition);

			/*sf::Vector2i movement = MovementToVector(cMovement->GetCurrentMovement());

			sf::Vector2i nextPosition = sf::Vector2i(cTransform->GetPosition().x + (movement.x * ((engine.CurrentScene()->GridSize() * 5) * engine.DeltaTime())),
				cTransform->GetPosition().y + (movement.y * ((engine.CurrentScene()->GridSize() * 5) * engine.DeltaTime())));

			sf::Vector2i position = nextPosition + sf::Vector2i(engine.CurrentScene()->GridHalfSize(), engine.CurrentScene()->GridHalfSize());
			sf::Vector2i movementVector = MovementToVector(cMovement->GetCurrentMovement());
			position = sf::Vector2i(position.x + (movementVector.x * engine.CurrentScene()->GridHalfSize()), position.y + (movementVector.y * engine.CurrentScene()->GridHalfSize()));
			sf::Vector2i nextGridPosition = PositionToGrid(position, engine.CurrentScene()->GridSize());

			auto* cPlayer = e->GetComponent<CPlayer>();
			if (cPlayer == nullptr || engine.CurrentScene()->GetNavGrid()[nextGridPosition.y * engine.CurrentScene()->GridColumns() + nextGridPosition.x] < NAV_GRID_BLOCKER)
			{
				cTransform->SetPosition(sf::Vector2f(nextPosition));
			}*/

		}
	}



	void SGridMovementSystem::SetDiagonalMovement(CTransform* ctransform, CGridMovement* cMovement, EMovement& movement)
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