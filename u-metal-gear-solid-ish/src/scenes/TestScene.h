#pragma once

#include "UScene.h"

class TestScene : public uei::UScene
{
public:
	TestScene(const uei::UEngine& inEngine) :
		uei::UScene(inEngine)
	{

	}

	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
};