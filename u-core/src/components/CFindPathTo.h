#pragma once
#include "UComponent.h"
#include <entities/UEntity.h>

namespace uei
{
	class CFindPathTo : public UComponent
	{
	public:

		CFindPathTo(/*const uei::UEntity& inTarget*/) : UComponent()
			/*target(inTarget)*/
		{
		}

		//uei::UEntity Target() { return target; }

	private:
		//const uei::UEntity target;
	};
}