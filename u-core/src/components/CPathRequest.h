#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
	class CPathRequest : public UComponent
	{
	public:

		CPathRequest(const sf::Vector2f& inTargetPosition) : UComponent(),
			targetPosition(inTargetPosition)
		{
		}

		sf::Vector2f TargetPosition() { return targetPosition; }

	private:
		const sf::Vector2f targetPosition;
	};
}
