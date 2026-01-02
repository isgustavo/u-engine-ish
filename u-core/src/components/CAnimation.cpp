#include "CAnimation.h"

namespace uei
{
	void CAnimation::Update()
	{
		currentAnimationFrame = (int)(++currentGameFrame / animationSpeed) % frameCount;
		currentSprite->setTextureRect(sf::IntRect({ currentAnimationFrame * frameSize.x, 0 }, frameSize));
	}
}