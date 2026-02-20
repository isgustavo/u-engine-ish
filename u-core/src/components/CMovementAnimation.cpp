#include "components/CMovementAnimation.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <imgui.h>
#include "CSprite.h"

namespace uei
{
	CMovementAnimation::CMovementAnimation() : UComponent(), animations()
	{
		for (int i = 0; i < AllLocomotionTypes().size(); ++i) 
		{
			animations.emplace(AllLocomotionTypes()[i], "");
		}
	}
	CMovementAnimation::~CMovementAnimation()
	{
	}
	void CMovementAnimation::OnShowEditor(UEngine& engine, std::function<void()> onRemove)
	{
		auto* assets = engine.Assets();
		const auto& allAnimationNames = assets->AnimationNames();

		if (!animations.empty())
		{
			int animationIndex = 0;
			for (auto& [key, value] : animations)
			{
				ImGui::Text(key.c_str());
				ImGui::SameLine();
				ImGui::PushID(++animationIndex);
				if (ImGui::Button("Clear"))
				{
					animations[key] = "";
				}
				ImGui::PopID();
				ImGui::PushItemWidth(260);
				std::string id = "##AnimationCombo_" + std::to_string(++animationIndex);
				if (ImGui::BeginCombo(id.c_str(), animations[key].c_str()))
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

				if (!(animations[key]).empty())
				{
					uei::AnimationAsset animationAsset = assets->Animation(animations[key]);

					currentAnimationDeltaTime += ImGui::GetIO().DeltaTime;
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
		return 180 + engine.CurrentScene()->GridSize() + animations.size() * 25;
	}

	void CMovementAnimation::OnComponentAdd(UEntity& entity)
	{
		auto* sprite = entity.GetComponent<CSprite>();
		if (sprite == nullptr)
		{
			sprite = new CSprite();
			entity.AddComponent(sprite);
		}

		sprite->SetRequiredByOtherComponent(true);

	}

	void CMovementAnimation::OnComponentRemove(UEntity& entity)
	{
		auto* sprite = entity.GetComponent<CSprite>();
		if (sprite != nullptr)
		{
			sprite->SetRequiredByOtherComponent(false);
		}
	}

	void CMovementAnimation::LoadComponent(UEngine& engine, std::istream& in)
	{
	}

	std::string CMovementAnimation::Save() const
	{
		return std::string();
	}
}