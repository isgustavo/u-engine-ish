#pragma once
#include "UComponent.h"

namespace uei
{
	class CObstacle : public UComponent
	{
	public:
		CObstacle();
		~CObstacle();

		UComponent* Clone() const override
		{
			return new CObstacle();
		}

		inline std::string ComponentName() const override { return "CObstacle"; }

		void LoadComponent(class UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}
