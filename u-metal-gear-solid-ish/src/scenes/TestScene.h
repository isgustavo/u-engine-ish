#pragma once

#include "UScene.h"
#include "UEngine.h"

class TestScene : public uei::UScene
{
public:
	TestScene(uei::UEngine& inEngine);

	virtual void OnStart() override;
	virtual void OnUpdate() override;
	virtual void OnDraw() override;
};