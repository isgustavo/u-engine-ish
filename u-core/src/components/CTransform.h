#pragma once
#include "UComponent.h"
#include <SFML/System/Vector2.hpp>

inline sf::Vector2i PositionToGrid(sf::Vector2f& position, int gridSize)
{
	return sf::Vector2i(position.x / gridSize, position.y / gridSize);
}

inline sf::Vector2i PositionToGrid(sf::Vector2i& position, int gridSize)
{
	return sf::Vector2i(position.x / gridSize, position.y / gridSize);
}

inline sf::Vector2i GridToPosition(sf::Vector2i& position, int gridSize)
{
	return sf::Vector2i(position.x * gridSize, position.y * gridSize);
}

inline float Distance(sf::Vector2i& positionA, sf::Vector2i& positionB)
{
	sf::Vector2i diff = positionB - positionA;
	return std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

namespace uei
{
	class CTransform : public UComponent
	{
	public:
		CTransform();
		~CTransform();

		sf::Vector2i& GetPosition() { return position; }
		sf::Vector2i& GetPositionLastUpdate() { return positionLastUpdate; }
		sf::Vector2i& GetGridPosition() { return gridPosition; }
		sf::Vector2i& GetPivotPosition() { return pivotPosition; }
		sf::Vector2i& GetPivotGridPosition() { return pivotGridPosition; }

		void SetPosition(const sf::Vector2i& inPosition);
		//bool IsMoving(const float threshold = 0.001f) const;
		//bool IsEqual(const sf::Vector2i& otherPosition, const float threshold = 0.001f) const;
		//float Distance(const sf::Vector2i& otherPosition) const;

		UComponent* Clone() override
		{
			return new CTransform(originalPosition);
		}

		virtual void Start(class UEngine& engine) override;

		inline std::string ComponentName() const override { return "CTransform"; }

	private:
		CTransform(const sf::Vector2i& inOriginalPosition);

		sf::Vector2i originalPosition;
		sf::Vector2i position;
		sf::Vector2i positionLastUpdate;
		sf::Vector2i gridPosition;
		sf::Vector2i pivotPosition;
		sf::Vector2i pivotGridPosition;
	};


}


