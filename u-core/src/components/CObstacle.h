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

	protected:
		void OnShowEditor(class UEngine& engine) override;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(UEntity& entity) override;

	};
}
