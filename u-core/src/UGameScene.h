#pragma once
#include "UEngine.h"

namespace uei
{
	class UGameScene : public UScene
	{
	public:
		UGameScene(UEngine& inEngine, std::string levelName);
		~UGameScene();

		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnDraw() override;
		virtual void OnMouseLeft() override;
		virtual void OnMouseRight() override;

	private:

	};
}