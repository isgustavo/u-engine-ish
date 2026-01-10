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
		CAnimation(const sf::Texture& inTexture, const int inFrameCount, const float inAnimationSpeed, 
					const int inFrameWidth, const int inFrameHeight, const sf::Vector2f inScale) : UComponent(),
			frameCount(inFrameCount), animationSpeed(inAnimationSpeed),
			frameSize({ inFrameWidth, inFrameHeight })
		{
			currentSprite = std::make_shared<sf::Sprite>(inTexture, sf::IntRect({ 0, 0 }, frameSize));
			currentSprite->scale(inScale);

			currentGameFrame = 0;
			currentAnimationFrame = 0;
			//currentSprite->setPosition({ 0, 90 }); //ToDo 
		}

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