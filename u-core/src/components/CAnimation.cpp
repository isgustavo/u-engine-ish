#include "CAnimation.h"

namespace uei
{
	CAnimation::CAnimation() : UComponent(),
		frameCount(0.f), animationSpeed(0.f), frameSize({ 0, 0 }), currentGameFrame(0), currentAnimationFrame(0)
	{

	}

	CAnimation::CAnimation(const sf::Texture& inTexture, const int inFrameCount, const float inAnimationSpeed,
		const int inFrameWidth, const int inFrameHeight, const sf::Vector2f inScale) : UComponent(),
		frameCount(inFrameCount), animationSpeed(inAnimationSpeed),
		frameSize({ inFrameWidth, inFrameHeight }), currentGameFrame(0), currentAnimationFrame(0)
	{
		currentSprite = std::make_shared<sf::Sprite>(inTexture, sf::IntRect({ 0, 0 }, frameSize));
		currentSprite->scale(inScale);

		//currentSprite->setPosition({ 0, 90 }); //ToDo 
	}

	void CAnimation::Update()
	{
		currentAnimationFrame = (int)(++currentGameFrame / animationSpeed) % frameCount;
		currentSprite->setTextureRect(sf::IntRect({ currentAnimationFrame * frameSize.x, 0 }, frameSize));
	}
}