#pragma once
#include "UComponent.h"

namespace uei 
{
	class CIdleAnimation : public UComponent
	{
	public:
		CIdleAnimation();
		~CIdleAnimation();

		std::string IdleAnimation() { return idleAnimation; }

		UComponent* Clone() const override
		{
			return new CIdleAnimation(idleAnimation);
		}

		inline std::string ComponentName() const override { return "CIdleAnimation"; }

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(UEngine& inEngine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		CIdleAnimation(std::string idleAnimation);

		std::string idleAnimation;

		float currentAnimationDeltaTime = 0.f;

		virtual void OnComponentAdd(UEntity& entity) override;
		virtual void OnComponentRemove(UEntity& entity) override;
	};

}