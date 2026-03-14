#pragma once
#include "CAnimation.h"
#include "UAsset.h"

namespace uei 
{
	class CIdleAnimation : public CAnimation
	{
	public:
		CIdleAnimation();
		~CIdleAnimation();

		std::string GetAnimationName() { return animationName; }
		AnimationAsset* GetAnimationAsset() { return animationAsset; }
		void SetAnimationAsset(AnimationAsset* inAnimationAsset) { animationAsset = inAnimationAsset; }
		//SpriteAsset GetCurrentAnimataionFrame(float deltaTime);

		UComponent* Clone() override
		{
			return new CIdleAnimation(animationName);
		}
		
		virtual void Start(UEngine& engine) override;

		inline std::string ComponentName() const override { return "CIdleAnimation"; }

		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(UEngine& inEngine) override;
		int GetEditorSize() const override;

		virtual void OnComponentAdd(UEntity& entity) override;
		virtual void OnComponentRemove(UEntity& entity) override;

	private:
		CIdleAnimation(std::string inAnimationName);

		AnimationAsset* animationAsset;

		std::string animationName;
	};
}