#include "SAnimationSystem.h"

#include "components/CTransform.h"
#include "components/CMovementAnimation.h"
#include "components/CIdleAnimation.h"
#include "components/CSprite.cpp"
#include <components/CMovement.h>

namespace uei
{
	SAnimationSystem::SAnimationSystem() : USystem(), currentAnimationDeltaTime(0.0f)
	{

	}

	void SAnimationSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		currentAnimationDeltaTime += engine.DeltaTime();
		for (auto& e : entities)
		{
			auto* cMovement = e->GetComponent<CMovement>();
			auto* cIdleAnimation = e->GetComponent<CIdleAnimation>();
			auto* cMovementAnimation = e->GetComponent<uei::CMovementAnimation>();
			auto* cSprite = e->GetComponent<uei::CSprite>();

			if (cMovement == nullptr || cIdleAnimation == nullptr || cMovementAnimation == nullptr || cSprite == nullptr) continue;

			if (cMovement->GetMovement() == EMovement::NONE)
			{
				if (cIdleAnimation->GetAnimationAsset() != nullptr)
				{
					cSprite->SetSpriteAsset(GetSprite(cIdleAnimation->GetAnimationAsset()));
				}
			}
			else 
			{
				std::string animationName = cMovementAnimation->GetAnimation(cMovement->GetMovement());
				const AnimationAsset* animationAsset = &engine.Assets()->GetAnimation(animationName);

				cSprite->SetSpriteAsset(GetSprite(animationAsset));
			}
		}
	}

	SpriteAsset* SAnimationSystem::GetSprite(const AnimationAsset* animationAsset)
	{
		int currentAnimationFrame = (int)(currentAnimationDeltaTime * animationAsset->Speed) % animationAsset->Frame;
		int x = animationAsset->X + (currentAnimationFrame * animationAsset->Width);
		return new SpriteAsset("", animationAsset->TextureName, x, animationAsset->Y, animationAsset->Width, animationAsset->Height);
	}


}