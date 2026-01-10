#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
	class CTransform : public UComponent
	{
	public:

		CTransform(const sf::Vector2f& inPosition/*, sf::Vector2f& inInitialVelocity*/) : UComponent(),
			position(inPosition), positionLastUpdate(position), bUpdate(true)/*, velocity(inInitialVelocity)*/
		{

		}

		sf::Vector2f& Position();
		void SetPosition(const sf::Vector2f& inPosition);
		//void SetVelocity(const sf::Vector2f& inVelocity);
		bool ShouldUpdate() const;

		//void Update(const float deltaTime);

	private:
		sf::Vector2f position;
		sf::Vector2f positionLastUpdate;
		bool bUpdate;
		//sf::Vector2f velocity;
		//Vec2f scale;
		//float angle;
	};
}
