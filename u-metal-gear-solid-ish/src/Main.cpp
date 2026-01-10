#include "UEngine.h"
#include <UScene.h>
#include "scenes/TestScene.h"

int main(int argc, char* argv[])
{
	auto engine = std::make_unique<uei::UEngine>(900, 900, "U-Metal Gear Solid-ish", 30,
		sf::Vector2f(90.f, 90.f), "Assets/fonts/OpenDyslexic-Regular.otf", "Assets/Assets.txt");
	
	engine->AddScene<TestScene>();
	engine->Start();
	engine->Update();
	return 0;
}