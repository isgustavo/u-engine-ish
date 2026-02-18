#pragma once
#include "UAsset.h"
#include "UScene.h"
#include "editor/UEditor.h"

#include <string>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>


inline const std::string LEVEL = "Level";
inline const std::string PREFAB = "Prefab";
inline const std::string TEXTURE = "Texture";
inline const std::string SPRITE = "Sprite";
inline const std::string ANIMATION = "Animation";
inline const std::string COMPONENT = "Component";
inline const std::string ENTITY = "Entity";

namespace uei
{
	class UEngine
	{
	public:
		UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit, bool showEditor = true);
		~UEngine();

		template<typename T>
		T* AddScene()
		{
			T* t = new T(*this);
			scenes.push_back(t);
			return t;
		}

		UAsset* Assets() { return assets; }
		UScene* CurrentScene() { return currentScene; }
		sf::RenderWindow& RenderWindow() { return renderWindow; }
		void Clear();
		sf::Vector2f& ScreenSize();

		void Start();
		void Update();

	private:

		sf::Vector2f screenSize;
		bool bShowEditor;

		sf::Font font;

		sf::RenderWindow renderWindow;

		UAsset* assets;
		UScene* currentScene;
		std::vector<UScene*> scenes;
	};
}