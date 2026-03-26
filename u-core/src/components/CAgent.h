#pragma once
#include "UComponent.h"

namespace uei
{
	class CAgent : public UComponent
	{
	public:

		CAgent();
		~CAgent();

		UComponent* Clone() override
		{
			return new CAgent();
		}

		inline std::string ComponentName() const override { return "CAgent"; }
	};
}