#pragma once
#include "UComponent.h"

namespace uei
{
	class CStaticDraw : public UComponent
	{
	public:
		CStaticDraw();
		~CStaticDraw();

		UComponent* Clone() override
		{
			return new CStaticDraw();
		}

		inline std::string ComponentName() const override { return "CStaticDraw"; }

	protected:
		void OnShowEditor(class UEngine& engine) override;

	};
}