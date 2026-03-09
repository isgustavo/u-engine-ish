#pragma once
#include "UComponent.h"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>
#include <UAsset.h>

namespace uei
{
	class CMovementAnimation : public UComponent
	{
	public:
		CMovementAnimation();
		~CMovementAnimation();

		UComponent* Clone() const override
		{
			return new CMovementAnimation(animations);
		}

		//std::vector<std::string>& AllLocomotionTypes()
		//{
		//	if (allLocomotionTypesName.size() == 0)
		//	{
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::LEFT));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::RIGHT));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP_LEFT));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN_LEFT));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN_RIGHT));
		//		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP_RIGHT));
		//	}
		//	return allLocomotionTypesName;
		//}

		std::string GetAnimation(EMovement movement);

		inline std::string ComponentName() const override { return "CMovementAnimation"; }

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;
	
	protected:
		void OnShowEditor(UEngine& inEngine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		CMovementAnimation(const std::unordered_map<std::string, std::string>& inAnimations);

		std::unordered_map<std::string, std::string> animations;

		float currentAnimationDeltaTime = 0.f;
		std::vector<std::string> allLocomotionTypesName;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(UEntity& entity) override;
	};
}