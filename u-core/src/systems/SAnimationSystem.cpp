#include "SAnimationSystem.h"

#include "components/CTransform.h"
#include "components/CMovementAnimation.h"
#include "components/CIdleAnimation.h"
#include "components/CSprite.cpp"
#include "components/CMovement.h"
#include "UEngine.h"

namespace uei
{
	SAnimationSystem::SAnimationSystem() : USystem()
	{

	}

	void uei::SAnimationSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		for (auto& e : entities)
		{
			auto* cIdleAnimation = e->GetComponent<CIdleAnimation>();
			auto* cMovementAnimation = e->GetComponent<CMovementAnimation>();

			if (cIdleAnimation == nullptr || cMovementAnimation == nullptr) continue;

			auto* cMovement = e->GetComponent<CMovement>();
			auto* cSprite = e->GetComponent<uei::CSprite>();

			if (cMovement == nullptr || cIdleAnimation == nullptr || cMovementAnimation == nullptr || cSprite == nullptr) continue;

			cIdleAnimation->UpdateAnimationDeltaTime(engine.DeltaTime());
			cMovementAnimation->UpdateAnimationDeltaTime(engine.DeltaTime());

			const AnimationAsset* animationAsset = nullptr;
			float animationDeltaTime;
			if (cMovement->GetCurrentMovement() == EMovement::NONE)
			{
				animationAsset = cIdleAnimation->GetAnimationAsset();
				animationDeltaTime = cIdleAnimation->GetCurrentAnimationDeltaTime();
			}
			else 
			{
				std::string animationName = cMovementAnimation->GetAnimation(cMovement->GetCurrentMovement());	
				animationAsset = &engine.Assets()->GetAnimationAsset(animationName);
				animationDeltaTime = cMovementAnimation->GetCurrentAnimationDeltaTime();
			}

			if(animationAsset != nullptr)
				cSprite->SetSpriteAsset(CreateAnimationSpriteAsset(animationAsset, animationDeltaTime));
		}
	}

	SpriteAsset* SAnimationSystem::CreateAnimationSpriteAsset(const AnimationAsset* animationAsset, const float deltaTime)
	{
		int currentAnimationFrame = (int)(deltaTime * animationAsset->Speed) % animationAsset->Frame;
		int x = animationAsset->X + (currentAnimationFrame * animationAsset->Width);
		return new SpriteAsset("", animationAsset->TextureName, x, animationAsset->Y, animationAsset->Width, animationAsset->Height);
	}


}