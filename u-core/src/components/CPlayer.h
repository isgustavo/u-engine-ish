#pragma once
#include "UComponent.h"

namespace uei
{
	class CPlayer : public UComponent
	{
	public:
		CPlayer();
		~CPlayer();

		UComponent* Clone() override
		{
			return new CPlayer();
		}

		inline std::string ComponentName() const override { return "CPlayer"; }

		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
	};
}