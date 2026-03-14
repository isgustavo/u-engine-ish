#include "components/CIdleAnimation.h"
#include "components/CAnimation.h"
#include "UEngine.h"
#include "UAsset.h"
#include "CSprite.h"

namespace uei
{
	CIdleAnimation::CIdleAnimation() : CAnimation()
	{
		animationName = EMPTY;
		animationAsset = nullptr;
	}

	CIdleAnimation::CIdleAnimation(std::string inAnimationName) : CIdleAnimation()
	{
		animationName = inAnimationName;
	}

	CIdleAnimation::~CIdleAnimation()
	{
		delete animationAsset;
		animationAsset = nullptr;
	}

	void CIdleAnimation::Start(UEngine& engine)
	{
		if (animationName != EMPTY)
			animationAsset = new AnimationAsset(engine.Assets()->GetAnimationAsset(animationName));
	}

	void CIdleAnimation::OnShowEditor(UEngine& engine)
	{
		auto* assets = engine.Assets();
		const auto& allAnimationNames = assets->AnimationNames();

		int animationIndex = 0;
		ImGui::Text("IDLE");
		ImGui::SameLine();
		ImGui::PushID(++animationIndex);
		if (ImGui::Button("Clear"))
		{
			animationAsset = nullptr;
			animationName = EMPTY;
		}
		ImGui::PopID();
		ImGui::PushItemWidth(260);
		std::string id = "##AnimationCombo_" + std::to_string(++animationIndex);
		if (ImGui::BeginCombo(id.c_str(), animationName.c_str()))
		{
			for (int i = 0; i < allAnimationNames.size(); ++i)
			{
				if (ImGui::Selectable(allAnimationNames[i].c_str()))
				{
					animationName = allAnimationNames[i];
					animationAsset = new AnimationAsset(assets->GetAnimationAsset(allAnimationNames[i]));
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();

		if (animationAsset != nullptr)
		{
			currentAnimationDeltaTime += engine.DeltaTime();
			int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset->Speed) % animationAsset->Frame;
			int spriteX = animationAsset->X + (currentAnimationFrame * animationAsset->Width);

			sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(animationAsset->TextureName),
				sf::IntRect({ spriteX, animationAsset->Y }, { animationAsset->Width, animationAsset->Height }));

			ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
			sf::Vector2u size = sprite->getTexture().getSize();

			ImVec2 uv0((float)spriteX / size.x, (float)animationAsset->Y / size.y);

			ImVec2 uv1((float)(spriteX + animationAsset->Width) / size.x,
				(float)(animationAsset->Y + animationAsset->Height) / size.y);

			ImGui::SameLine();
			ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
		}
	}

	int uei::CIdleAnimation::GetEditorSize() const
	{
		return 150;
	}

	void CIdleAnimation::OnComponentAdd(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(true);
	}

	void CIdleAnimation::OnComponentRemove(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CSprite>(false);
	}

	void uei::CIdleAnimation::LoadComponent(std::istream& in)
	{
		Deserialize(in, animationName);
	}

	std::string CIdleAnimation::SaveComponent() const
	{
		return Serialize(animationName);
	}
}