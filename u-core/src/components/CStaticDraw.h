#pragma once
#include "UComponent.h"

namespace uei
{
	class CStaticDraw : public UComponent
	{
	public:
		CStaticDraw();
		~CStaticDraw();

		UComponent* Clone() const override
		{
			return new CStaticDraw();
		}

		inline std::string ComponentName() const override { return "CStaticDraw"; }

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