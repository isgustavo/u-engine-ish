#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "UComponent.h"
#include <memory>

namespace uei
{
	class CSprite : public UComponent
	{
	public:
		CSprite() : UComponent(false) {}

		CSprite(const sf::Texture& texture)
			: UComponent(true)
		{
			sprite = std::make_shared<sf::Sprite>(texture);
		}

		sf::Sprite& GetSprite() { return *sprite.get(); }

	private:
		std::shared_ptr<sf::Sprite> sprite;
	};
}