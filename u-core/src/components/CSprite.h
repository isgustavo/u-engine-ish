#pragma once
#include "UComponent.h"

#include <SFML/Graphics/Sprite.hpp>
#include <memory>

namespace uei
{
	class CSprite : public UComponent
	{
	public:

		CSprite(const sf::Texture& texture, const int x, const int y, const int width, const int height, const sf::Vector2f scale)
			: UComponent(true)
		{
			sprite = std::make_shared<sf::Sprite>(texture, sf::IntRect({ x, y }, { width, height }));
			sprite->scale(scale);
			sprite->setPosition({0, 0}); //ToDo 
		}

		sf::Sprite& GetSprite() { return *sprite.get(); }

	private:
		std::shared_ptr<sf::Sprite> sprite;
	};
}