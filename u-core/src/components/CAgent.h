#pragma once
#include "CRect.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace uei
{
	class CAgent : public CRect
	{
	public:

		CAgent(const sf::Vector2f& inSize, const sf::Vector2i& inGridSize) : CRect(inSize, sf::Color::Green),
			gridSize(inGridSize)
		{
		}

		const sf::Vector2i& GridSize() { return gridSize; }

	private:
		sf::Vector2i gridSize;
	};
}