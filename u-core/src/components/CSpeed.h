#pragma once
#include "UComponent.h"

namespace uei
{
	class CSpeed : public UComponent
	{
	public:

		CSpeed(const float inSpeed) : UComponent(),
			speed(inSpeed)
		{

		}

		float Speed() { return speed; }

	private:
		float speed;
	};
}
