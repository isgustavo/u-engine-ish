#include "components/CMovementAnimation.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <sstream>
#include <imgui.h>
#include "CSprite.h"
#include "CMovement.h"
#include "CIdleAnimation.h"

namespace uei
{
	CMovementAnimation::CMovementAnimation() : CAnimation(), animations()
	{
		allLocomotionTypesName.push_back(MovementToString(EMovement::UP));
		allLocomotionTypesName.push_back(MovementToString(EMovement::LEFT));
		allLocomotionTypesName.push_back(MovementToString(EMovement::DOWN));
		allLocomotionTypesName.push_back(MovementToString(EMovement::RIGHT));
		allLocomotionTypesName.push_back(MovementToString(EMovement::UP_LEFT));
		allLocomotionTypesName.push_back(MovementToString(EMovement::DOWN_LEFT));
		allLocomotionTypesName.push_back(MovementToString(EMovement::DOWN_RIGHT));
		allLocomotionTypesName.push_back(MovementToString(EMovement::UP_RIGHT));

		for (auto& key : allLocomotionTypesName)
		{
			animations[key] = EMPTY;
		}
	}

	CMovementAnimation::CMovementAnimation(const std::unordered_map<std::string, std::string>& inAnimations) : CMovementAnimation()
	{
		for (auto& [key, value] : inAnimations)
		{
			animations[key] = value;
		}
	}

	CMovementAnimation::~CMovementAnimation()
	{

	}
	
	void CMovementAnimation::OnShowEditor(UEngine& engine)
	{
		auto* assets = engine.Assets();
		const auto& allAnimationNames = assets->AnimationNames();

		int animationIndex = 0;

		for (const std::string& key : allLocomotionTypesName)
		{
			auto animationName = animations.find(key);
			std::string value = " ";
			if (animationName != animations.end())
			{
				value = animationName->second;
			}

			ImGui::Text(key.c_str());
			ImGui::SameLine();
			ImGui::PushID(++animationIndex);
			if (ImGui::Button("Clear"))
			{
				animations[key] = EMPTY;
			}
			ImGui::PopID();
			ImGui::PushItemWidth(260);
			std::string id = "##AnimationCombo_" + std::to_string(++animationIndex);
			if (ImGui::BeginCombo(id.c_str(), value.c_str()))
			{
				for (int i = 0; i < allAnimationNames.size(); ++i)
				{
					if (ImGui::Selectable(allAnimationNames[i].c_str()))
					{
						animations[key] = allAnimationNames[i];
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			if (!(animations[key] == EMPTY))
			{
				AnimationAsset animationAsset = assets->GetAnimationAsset(animations[key]);

				currentAnimationDeltaTime += engine.DeltaTime();
				int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset.Speed) % animationAsset.Frame;
				int spriteX = animationAsset.X + (currentAnimationFrame * animationAsset.Width);

				sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(animationAsset.TextureName),
					sf::IntRect({ spriteX, animationAsset.Y }, { animationAsset.Width, animationAsset.Height }));

				ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
				sf::Vector2u size = sprite->getTexture().getSize();

				ImVec2 uv0(
					(float)spriteX / size.x,
					(float)animationAsset.Y / size.y);

				ImVec2 uv1(
					(float)(spriteX + animationAsset.Width) / size.x,
					(float)(animationAsset.Y + animationAsset.Height) / size.y);

				ImGui::SameLine();
				ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
			}
		}
	}

	int uei::CMovementAnimation::GetEditorSize() const
	{
		return 380;
	}

	void CMovementAnimation::OnComponentAdd(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(true);
		entity.SetRequiredByOtherComponent<CIdleAnimation>(true);
		entity.SetRequiredByOtherComponent<CMovement>(true);
	}

	void CMovementAnimation::OnComponentRemove(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(false);
		entity.SetRequiredByOtherComponent<CIdleAnimation>(false);
		entity.SetRequiredByOtherComponent<CMovement>(false);
	}

	std::string CMovementAnimation::GetAnimation(EMovement movement)
	{
		auto it = animations.find(MovementToString(movement));
		return it != animations.end() ? it->second : EMPTY;
	}

	void uei::CMovementAnimation::LoadComponent(std::istream& in)
	{
		/*std::string a0, a1, a2, a3, a4, a5, a6, a7;
		in >> a0 >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7;

		animations[allLocomotionTypesName[0]] = a0;
		animations[allLocomotionTypesName[1]] = a1;
		animations[allLocomotionTypesName[2]] = a2;
		animations[allLocomotionTypesName[3]] = a3;
		animations[allLocomotionTypesName[4]] = a4;
		animations[allLocomotionTypesName[5]] = a5;
		animations[allLocomotionTypesName[6]] = a6;
		animations[allLocomotionTypesName[7]] = a7;*/

		Deserialize(in, animations[allLocomotionTypesName[0]], 
			animations[allLocomotionTypesName[1]],
			animations[allLocomotionTypesName[2]], 
			animations[allLocomotionTypesName[3]], 
			animations[allLocomotionTypesName[4]], 
			animations[allLocomotionTypesName[5]], 
			animations[allLocomotionTypesName[6]],
			animations[allLocomotionTypesName[7]]);
	}

	std::string CMovementAnimation::SaveComponent() const
	{
		std::stringstream ss;
		for (const std::string& key : allLocomotionTypesName)
		{
			auto it = animations.find(key);
			ss << (it != animations.end() ? it->second : EMPTY) << " ";
		}

		return ss.str();
	}
}