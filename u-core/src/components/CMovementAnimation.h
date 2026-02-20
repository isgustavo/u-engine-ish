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
			return nullptr;
		}

		std::vector<std::string>& AllLocomotionTypes()
		{
			if (allLocomotionTypesName.size() == 0)
			{
				//allLocomotionTypesName.push_back(" ");
				allLocomotionTypesName.push_back("UP");
				allLocomotionTypesName.push_back("LEFT");
				allLocomotionTypesName.push_back("DOWN");
				allLocomotionTypesName.push_back("RIGHT");
				allLocomotionTypesName.push_back("UP_LEFT");
				allLocomotionTypesName.push_back("DOWN_LEFT");
				allLocomotionTypesName.push_back("DOWN_RIGHT");
				allLocomotionTypesName.push_back("UP_RIGHT");
			}
			return allLocomotionTypesName;
		}

		inline std::string ComponentName() const override { return "CMovementAnimation"; }

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;
	
	protected:
		void OnShowEditor(UEngine& inEngine, std::function<void()> onRemove) override;
		int GetEditorSize(UEngine& engine) const override;

	private:

		std::unordered_map<std::string, std::string> animations;

		float currentAnimationDeltaTime = 0.f;
		std::vector<std::string> allLocomotionTypesName;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(UEntity& entity) override;
	};
}