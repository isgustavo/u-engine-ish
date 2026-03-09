#pragma once
#include "UComponent.h"
#include "UAsset.h"

namespace uei 
{
	class CIdleAnimation : public UComponent
	{
	public:
		CIdleAnimation();
		~CIdleAnimation();

		//std::string GetIdleAnimationName() { return idleAnimationName; }
		AnimationAsset* GetAnimationAsset() { return animationAsset; }
		uei::SpriteAsset GetCurrentAnimataionFrame(float deltaTime);

		UComponent* Clone() const override
		{
			return new CIdleAnimation(animationAsset);
		}

		inline std::string ComponentName() const override { return "CIdleAnimation"; }

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(UEngine& inEngine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		CIdleAnimation(AnimationAsset* inAnimationAsset);

		AnimationAsset* animationAsset;
		//std::string idleAnimationName;

		float currentAnimationDeltaTime = 0.f;

		virtual void OnComponentAdd(UEntity& entity) override;
		virtual void OnComponentRemove(UEntity& entity) override;
	};
}