#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <memory>

namespace uei
{
	class CCircle : public UComponent
	{
	public:

		CCircle(const sf::Color inInitialColor, const float inRadius, const int inPoints) : UComponent(),
			initialColor(inInitialColor)
		{
			circle = std::make_unique<sf::CircleShape>(inRadius, inPoints);
			circle.get()->setFillColor(initialColor);
		}

		sf::CircleShape& Circle() { return *circle; }

	private:
		const sf::Color initialColor;
		std::unique_ptr<sf::CircleShape> circle;
	};
}
