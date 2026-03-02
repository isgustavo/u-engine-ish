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
	CMovementAnimation::CMovementAnimation() : UComponent(), animations() { }

	CMovementAnimation::CMovementAnimation(const std::unordered_map<std::string, std::string>& inAnimations) : UComponent()
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
			for (const std::string& key : AllLocomotionTypes())
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
					uei::AnimationAsset animationAsset = assets->Animation(animations[key]);

					currentAnimationDeltaTime += ImGui::GetIO().DeltaTime; // ToDo delta time
					int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset.speed) % animationAsset.frame;
					int spriteX = animationAsset.x + (currentAnimationFrame * animationAsset.width);

					sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(animationAsset.textureName),
						sf::IntRect({ spriteX, animationAsset.y }, { animationAsset.width, animationAsset.height }));

					ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
					sf::Vector2u size = sprite->getTexture().getSize();

					//ImGui::Spacing();

					ImVec2 uv0(
						(float)spriteX / size.x,
						(float)animationAsset.y / size.y);

					ImVec2 uv1(
						(float)(spriteX + animationAsset.width) / size.x,
						(float)(animationAsset.y + animationAsset.height) / size.y);

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

	void CMovementAnimation::LoadComponent(UEngine& engine, std::istream& in)
	{
		std::string a0, a1, a2, a3, a4, a5, a6, a7;
		in >> a0 >> a1 >> a2 >> a3 >> a4 >> a5 >> a6 >> a7;

		animations[AllLocomotionTypes()[0]] = a0;
		animations[AllLocomotionTypes()[1]] = a1;
		animations[AllLocomotionTypes()[2]] = a2;
		animations[AllLocomotionTypes()[3]] = a3;
		animations[AllLocomotionTypes()[4]] = a4;
		animations[AllLocomotionTypes()[5]] = a5;
		animations[AllLocomotionTypes()[6]] = a6;
		animations[AllLocomotionTypes()[7]] = a7;
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