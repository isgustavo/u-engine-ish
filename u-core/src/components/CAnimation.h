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
		~CAnimation();

		float GetCurrentAnimationDeltaTime() const { return currentAnimationDeltaTime; }
		void UpdateAnimationDeltaTime(float deltaTime) { currentAnimationDeltaTime += deltaTime; }

	protected:
		float currentAnimationDeltaTime = 0.f;
	};
}