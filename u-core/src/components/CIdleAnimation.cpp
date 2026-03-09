#include "components/CIdleAnimation.h"
#include "components/UComponent.h"
#include "UEngine.h"
#include "UAsset.h"
#include "CSprite.h"

namespace uei
{
	CIdleAnimation::CIdleAnimation() : UComponent()
	{
		currentAnimationDeltaTime = 0.f;
	}

	CIdleAnimation::CIdleAnimation(AnimationAsset* inAnimationAsset) : UComponent(),
		animationAsset(inAnimationAsset), currentAnimationDeltaTime(0.f)
	{

	}

	CIdleAnimation::~CIdleAnimation()
	{
		delete animationAsset;
		animationAsset = nullptr;
	}

	SpriteAsset CIdleAnimation::GetCurrentAnimataionFrame(float deltaTime)
	{
		currentAnimationDeltaTime += deltaTime;
		int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset->Speed) % animationAsset->Frame;

		return SpriteAsset(EMPTY, animationAsset->TextureName, 
			animationAsset->X + (currentAnimationFrame * animationAsset->Width), animationAsset->Y, 
			animationAsset->Width, animationAsset->Height);
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
				animationAsset = nullptr;
				//idleAnimationName = "";
			}
			ImGui::PopID();
			ImGui::PushItemWidth(260);
			std::string id = "##AnimationCombo_" + std::to_string(++animationIndex);
			if (ImGui::BeginCombo(id.c_str(), (animationAsset == nullptr) ? " " : animationAsset->AssetName.c_str()))
			{
				for (int i = 0; i < allAnimationNames.size(); ++i)
				{
					if (ImGui::Selectable(allAnimationNames[i].c_str()))
					{
						animationAsset = new AnimationAsset(assets->GetAnimation(allAnimationNames[i]));
						//idleAnimationName = allAnimationNames[i];
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}

		if (animationAsset != nullptr)
		{
			//uei::AnimationAsset animationAsset = assets->GetAnimation(idleAnimationName);

			currentAnimationDeltaTime += engine.DeltaTime();// ImGui::GetIO().DeltaTime;
			int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset->Speed) % animationAsset->Frame;
			int spriteX = animationAsset->X + (currentAnimationFrame * animationAsset->Width);

			sf::Sprite* sprite = new sf::Sprite(engine.Assets()->GetTexture(animationAsset->TextureName),
				sf::IntRect({ spriteX, animationAsset->Y }, { animationAsset->Width, animationAsset->Height }));

			ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
			sf::Vector2u size = sprite->getTexture().getSize();

			//ImGui::Spacing();

			ImVec2 uv0((float)spriteX / size.x, (float)animationAsset->Y / size.y);

			ImVec2 uv1((float)(spriteX + animationAsset->Width) / size.x,
				(float)(animationAsset->Y + animationAsset->Height) / size.y);

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
		std::string animationName;

		in >> animationName;

		if (animationName != EMPTY)
		{
			animationAsset = new AnimationAsset(engine.Assets()->GetAnimation(animationName));
		}
	}

	std::string CIdleAnimation::Save() const
	{
		return animationAsset == nullptr ? EMPTY : animationAsset->AssetName;
	}
}