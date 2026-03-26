#pragma once
#include "UAsset.h"
#include "UScene.h"
#include "editor/UEditor.h"

#include <string>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
//#include <SFML/Graphics/Font.hpp>
#include <iostream>

inline const std::string LEVEL = "Level";
inline const std::string PREFAB = "Prefab";
inline const std::string TEXTURE = "Texture";
inline const std::string SPRITE = "Sprite";
inline const std::string ANIMATION = "Animation";
inline const std::string FONT = "Font";
inline const std::string SYSTEM = "System";
inline const std::string COMPONENT = "Component";
inline const std::string ENTITY = "Entity";
inline const std::string EMPTY = "Empty";

namespace uei
{
	class UEngine
	{
	public:
		UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit);
		~UEngine();

		UAsset* Assets() { return assets; }
		UScene* CurrentScene() { return currentScene; }
		void SetScene(UScene* scene) 
		{ 
			scenes.push_back(scene);
			currentScene = scene;
		}
		sf::RenderWindow& RenderWindow() { return renderWindow; }

		void Start();
		void Update();
		void Clear();

		const float DeltaTime() const { return fixedDeltaTime; }
		const sf::Vector2f& GetScreenSize() { return screenSize; }

	private:
		UAsset* assets;
		UScene* currentScene;
		sf::RenderWindow renderWindow;
		std::vector<UScene*> scenes;

		sf::Vector2f screenSize;
		const float fixedDeltaTime;
	};
}