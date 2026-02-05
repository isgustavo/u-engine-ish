#pragma once
#include "UEngine.h"

namespace uei
{
	class UEditorScene : public UScene
	{
	public:
		UEditorScene(UEngine& inEngine);

		virtual void OnStart() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		virtual void Editor_Show() override;

	private:
		sf::View view;

		
	};
}