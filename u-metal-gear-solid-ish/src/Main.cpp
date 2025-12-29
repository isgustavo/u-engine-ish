#include "UEngine.h"
#include <UScene.h>
#include "scenes/TestScene.h"

int main(int argc, char* argv[])
{
	auto engine = std::make_unique<uei::UEngine>(640, 640, "U-Metal Gear Solid-ish", 30,
		uei::UFloat2D(64, 64), "Assets/fonts/OpenDyslexic-Regular.otf", "Assets/Assets.txt");
	
	engine->AddScene<TestScene>();
	engine->Start();
	engine->Update();
	return 0;
}