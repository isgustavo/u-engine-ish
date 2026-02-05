#pragma once
#include "UComponent.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

namespace uei
{
	class CAnimation : public UComponent
	{
	public:
		CAnimation();
		CAnimation(const sf::Texture& inTexture, const int inFrameCount, const float inAnimationSpeed,
			const int inFrameWidth, const int inFrameHeight, const sf::Vector2f inScale);

		//std::unique_ptr<UComponent> Clone() const override
		//{
		//	return std::make_unique<CAnimation>(this->);
		//}

		void Update();
		sf::Sprite& CurrentSprite() { return *currentSprite.get(); }

	private:
		const int frameCount;
		const float animationSpeed;
		const sf::Vector2i frameSize;
		std::shared_ptr<sf::Sprite> currentSprite;

		int currentGameFrame;
		int currentAnimationFrame;	
	};
}