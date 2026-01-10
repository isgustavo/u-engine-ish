#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace uei
{
	class CRect : public UComponent
	{
	public:

		CRect(const sf::Vector2f& inSize, const sf::Color inInitialColor) : UComponent(),
			size(inSize), initialColor(inInitialColor)
		{
			rect = std::make_unique<sf::RectangleShape>(inSize);
			rect.get()->setFillColor(initialColor);
		}
		
		sf::RectangleShape& Rect() { return *rect; }

	private:
		const sf::Vector2f size;
		const sf::Color initialColor;
		std::unique_ptr<sf::RectangleShape> rect;
	};
}