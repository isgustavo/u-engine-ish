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
		return EMovement::DOWN;
	if (movement == "DOWN")
		return EMovement::UP;
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

		std::vector<std::string> allLocomotionTypesName;
	};
}