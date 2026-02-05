#pragma once
#include "CRect.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace uei
{
	class CTarget : public UComponent
	{
	public:

		CTarget();

		virtual void Editor_Show(UEngine& inEngine, bool bIsNew) override;
	};
}
