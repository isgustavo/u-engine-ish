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
	CMovementAnimation::CMovementAnimation() : UComponent(), animations() 
	{
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::LEFT));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::RIGHT));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP_LEFT));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN_LEFT));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::DOWN_RIGHT));
		allLocomotionTypesName.push_back(uei::MovementToString(EMovement::UP_RIGHT));

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

		//if (!animations.empty())
		{
			int animationIndex = 0;
			//for (auto& [key, value] : animations)
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

				//ImGui::Checkbox("FlipX", &bFlipX);
				//ImGui::SameLine();
				//ImGui::Checkbox("FlipY", &bFlipY);

				if (!(animations[key] == EMPTY))
				{
					AnimationAsset animationAsset = assets->GetAnimation(animations[key]);

					currentAnimationDeltaTime += engine.DeltaTime();// ImGui::GetIO().DeltaTime; // ToDo delta time
					int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset.Speed) % animationAsset.Frame;
					int spriteX = animationAsset.X + (currentAnimationFrame * animationAsset.Width);

					sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(animationAsset.TextureName),
						sf::IntRect({ spriteX, animationAsset.Y }, { animationAsset.Width, animationAsset.Height }));

					ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
					sf::Vector2u size = sprite->getTexture().getSize();

					//ImGui::Spacing();

					ImVec2 uv0(
						(float)spriteX / size.x,
						(float)animationAsset.Y / size.y);

					ImVec2 uv1(
						(float)(spriteX + animationAsset.Width) / size.x,
						(float)(animationAsset.Y + animationAsset.Height) / size.y);

					ImGui::SameLine();
					ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
				}
				//ImGui::EndChild();
			}
		}
	}

	int CMovementAnimation::GetEditorSize(UEngine& engine) const
	{
		return bIsRequiredByOtherComponent ? 33 : 180 + engine.CurrentScene()->GridSize() + (animations.size() * 25);
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
		auto it = animations.find(uei::MovementToString(movement));
		return it != animations.end() ? it->second : EMPTY;
	}

	void CMovementAnimation::LoadComponent(UEngine& engine, std::istream& in)
	{
		std::string a0, a1, a2, a3, a4, a5, a6, a7;
		in >> a0 >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7;

		animations[allLocomotionTypesName[0]] = a0;
		animations[allLocomotionTypesName[1]] = a1;
		animations[allLocomotionTypesName[2]] = a2;
		animations[allLocomotionTypesName[3]] = a3;
		animations[allLocomotionTypesName[4]] = a4;
		animations[allLocomotionTypesName[5]] = a5;
		animations[allLocomotionTypesName[6]] = a6;
		animations[allLocomotionTypesName[7]] = a7;
	}

	std::string CMovementAnimation::Save() const
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