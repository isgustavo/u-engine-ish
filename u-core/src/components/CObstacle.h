#pragma once
#include "UComponent.h"

namespace uei
{
	class CObstacle : public UComponent
	{
	public:
		CObstacle();
		~CObstacle();

		UComponent* Clone() override
		{
			return new CObstacle();
		}

		inline std::string ComponentName() const override { return "CObstacle"; }

		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;

	};
}
