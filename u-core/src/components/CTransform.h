#pragma once
#include "UComponent.h"
#include "math/UFloat2D.h"

namespace uei
{
	class CTransform : public UComponent
	{
	public:

		CTransform(const uei::UFloat2D& inPosition) : UComponent(true),
			position(inPosition), positionLastUpdate(inPosition) {
		}

	private:
		uei::UFloat2D position;
		uei::UFloat2D positionLastUpdate;
		uei::UFloat2D velocity;
		//Vec2f scale;
		//float angle;
	};
}
