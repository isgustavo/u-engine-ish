#include "components/CIdleAnimation.h"
#include "components/UComponent.h"
#include "UEngine.h"
#include "CSprite.h"

namespace uei
{
	CIdleAnimation::CIdleAnimation() : UComponent()
	{
	}

	CIdleAnimation::CIdleAnimation(std::string inIdleAnimation) : UComponent()
	{
		idleAnimation = inIdleAnimation;
	}

	CIdleAnimation::~CIdleAnimation()
	{
	}

	void CIdleAnimation::OnShowEditor(UEngine& engine)
	{
		auto* assets = engine.Assets();
		const auto& allAnimationNames = assets->AnimationNames();

		//if (idleAnimation.empty())
		{
			int animationIndex = 0;
			ImGui::Text("IDLE");
			ImGui::SameLine();
			ImGui::PushID(++animationIndex);
			if (ImGui::Button("Clear"))
			{
				idleAnimation = "";
			}
			ImGui::PopID();
			ImGui::PushItemWidth(260);
			std::string id = "##AnimationCombo_" + std::to_string(++animationIndex);
			if (ImGui::BeginCombo(id.c_str(), idleAnimation.c_str()))
			{
				for (int i = 0; i < allAnimationNames.size(); ++i)
				{
					if (ImGui::Selectable(allAnimationNames[i].c_str()))
					{
						idleAnimation = allAnimationNames[i];
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}

		if (!idleAnimation.empty())
		{
			uei::AnimationAsset animationAsset = assets->Animation(idleAnimation);

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
	}

	int CIdleAnimation::GetEditorSize(UEngine& engine) const
	{
		return 120 + engine.CurrentScene()->GridSize();
	}

	void CIdleAnimation::OnComponentAdd(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(true);
	}

	void CIdleAnimation::OnComponentRemove(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(false);
	}

	void CIdleAnimation::LoadComponent(UEngine& engine, std::istream& in)
	{
		in >> idleAnimation;
	}

	std::string CIdleAnimation::Save() const
	{
		return idleAnimation.empty() ? EMPTY : idleAnimation;
	}
}