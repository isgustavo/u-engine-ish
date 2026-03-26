#pragma once
#include "CAnimation.h"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>

enum class EMovement
{
	NONE,
	UP,
	LEFT,
	DOWN,
	RIGHT,
	UP_LEFT,
	DOWN_LEFT,
	DOWN_RIGHT,
	UP_RIGHT
};

inline std::string MovementToString(EMovement movement)
{
	switch (movement)
	{
	case EMovement::UP:
		return "UP";
	case EMovement::LEFT:
		return "LEFT";
	case EMovement::DOWN:
		return "DOWN";
	case EMovement::RIGHT:
		return "RIGHT";
	case EMovement::UP_LEFT:
		return "UP_LEFT";
	case EMovement::DOWN_LEFT:
		return "DOWN_LEFT";
	case EMovement::DOWN_RIGHT:
		return "DOWN_RIGHT";
	case EMovement::UP_RIGHT:
		return "UP_RIGHT";
	default:
		return "NONE";
	}
}

inline EMovement StringToMovement(std::string movement)
{
	if (movement == "UP")
		return EMovement::UP;
	if (movement == "LEFT")
		return EMovement::LEFT;
	if (movement == "DOWN")
		return EMovement::DOWN;
	if (movement == "RIGHT")
		return EMovement::RIGHT;
	if (movement == "UP_LEFT")
		return EMovement::UP_LEFT;
	if (movement == "DOWN_LEFT")
		return EMovement::DOWN_LEFT;
	if (movement == "DOWN_RIGHT")
		return EMovement::DOWN_RIGHT;
	if (movement == "UP_RIGHT")
		return EMovement::UP_RIGHT;
	return EMovement::NONE;
}

inline sf::Vector2i MovementToVector(EMovement movement)
{
	switch (movement)
	{
	case EMovement::DOWN:
		return sf::Vector2i(0, 1);
	case EMovement::LEFT:
		return sf::Vector2i(-1, 0);
	case EMovement::UP:
		return sf::Vector2i(0, -1);
	case EMovement::RIGHT:
		return sf::Vector2i(1, 0);
	case EMovement::UP_LEFT:
		return sf::Vector2i(-1, 1);
	case EMovement::DOWN_LEFT:
		return sf::Vector2i(-1, -1);
	case EMovement::DOWN_RIGHT:
		return sf::Vector2i(1, -1);
	case EMovement::UP_RIGHT:
		return sf::Vector2i(1, 1);
	default:
		return sf::Vector2i(0, 0);
	}
}

inline EMovement InverseMovement(EMovement movement)
{
	switch (movement)
	{
	case EMovement::DOWN:
		return EMovement::UP;
	case EMovement::LEFT:
		return EMovement::RIGHT;
	case EMovement::UP:
		return EMovement::DOWN;
	case EMovement::RIGHT:
		return EMovement::LEFT;
	case EMovement::UP_LEFT:
		return EMovement::DOWN_RIGHT;
	case EMovement::DOWN_LEFT:
		return EMovement::UP_RIGHT;
	case EMovement::DOWN_RIGHT:
		return EMovement::UP_LEFT;
	case EMovement::UP_RIGHT:
		return EMovement::DOWN_LEFT;
	default:
		return EMovement::NONE;
	}
}

inline EMovement VectorToMovement(sf::Vector2i& movement)
{
	if (movement.x == 0 && movement.y == 1) return EMovement::DOWN;
	if (movement.x == -1 && movement.y == 0) return EMovement::LEFT;
	if (movement.x == 0 && movement.y == -1) return EMovement::UP;
	if (movement.x == 1 && movement.y == 0) return EMovement::RIGHT;
	if (movement.x == -1 && movement.y == 1) return EMovement::UP_LEFT;
	if (movement.x == -1 && movement.y == -1) return EMovement::DOWN_LEFT;
	if (movement.x == 1 && movement.y == -1) return EMovement::DOWN_RIGHT;
	if (movement.x == 1 && movement.y == 1) return EMovement::UP_RIGHT;

	return EMovement::NONE;
}

namespace uei
{
	class CMovementAnimation : public CAnimation
	{
	public:
		CMovementAnimation();
		~CMovementAnimation();

		std::string GetAnimation(EMovement movement);


		UComponent* Clone() override
		{
			return new CMovementAnimation(animations);
		}

		inline std::string ComponentName() const override { return "CMovementAnimation"; }

		void LoadComponent(std::istream& in) override;
		std::string SaveComponent() const override;
	
	protected:
		void OnShowEditor(UEngine& inEngine) override;
		int GetEditorSize() const override;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(UEntity& entity) override;

	private:
		CMovementAnimation(const std::unordered_map<std::string, std::string>& inAnimations);

		std::unordered_map<std::string, std::string> animations;


	};
}