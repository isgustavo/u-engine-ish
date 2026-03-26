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
	};
}