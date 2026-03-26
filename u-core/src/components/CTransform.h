#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

inline sf::Vector2i PositionToGrid(sf::Vector2f& position, int gridSize)
{
	return sf::Vector2i((int)((position.x + (gridSize * 0.5f)) / gridSize), (int)((position.y + (gridSize * 0.5f)) / gridSize));
}

inline sf::Vector2f PositionToCenter(sf::Vector2f position, int gridSize)
{
	return sf::Vector2f(position.x * gridSize + gridSize * 0.5f, position.y * gridSize + gridSize * 0.5f);
}

inline sf::Vector2f GridToPosition(sf::Vector2i& position, int gridSize)
{
	return sf::Vector2f(position.x * gridSize + gridSize * 0.5f, position.y * gridSize + gridSize * 0.5f);
}

namespace uei
{
	class CTransform : public UComponent
	{
	public:
		CTransform();
		~CTransform();

		sf::Vector2f& GetPosition();
		sf::Vector2f& GetPositionLastUpdate();
		void SetPosition(const sf::Vector2f& inPosition);
		bool IsMoving(const float threshold = 0.001f) const;
		bool IsEqual(const sf::Vector2f& otherPosition, const float threshold = 0.001f) const;

		UComponent* Clone() override
		{
			return new CTransform(originalPosition, position);
		}

		virtual void Start(class UEngine& engine) override;

		inline std::string ComponentName() const override { return "CTransform"; }

	private:
		CTransform(const sf::Vector2f& inOriginalPosition, const sf::Vector2f& inPosition);

		sf::Vector2f originalPosition;
		sf::Vector2f position;
		sf::Vector2f positionLastUpdate;

	};


}


