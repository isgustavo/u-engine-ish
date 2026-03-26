#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
	class CPathRequest : public UComponent
	{
	public:
		CPathRequest();
		~CPathRequest();

		CPathRequest* Clone() override
		{
			return new CPathRequest(targetPosition);
		}

		inline std::string ComponentName() const override { return "CPathRequest"; }

		void SetTargetPosition(sf::Vector2f& inTargetPosition) { targetPosition = inTargetPosition; }
		sf::Vector2f& TargetPosition() { return targetPosition; }
		std::vector<sf::Vector2i>& GetInitialInvalidGridMovement() { return initialInvalidGridMovement; }
		void SetInitialInvalidGridMovement(std::vector<sf::Vector2i>& validGridMovement) { initialInvalidGridMovement = validGridMovement; }

	private:
		CPathRequest(sf::Vector2f& inTargetPosition);

		sf::Vector2f targetPosition;
		std::vector<sf::Vector2i> initialInvalidGridMovement;
	};
}
