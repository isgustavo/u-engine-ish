#pragma once
#include "math/UFloat2D.h"
#include "UAsset.h"
#include "UScene.h"

#include <string>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	class UEngine
	{
	public:
		UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit, 
			uei::UFloat2D inGridSize, const std::string& fontPath, const std::string& assetFilePath)
			: width(inWidth), height(inHeight), bShowGrid(false), gridSize(inGridSize), renderWindow(sf::VideoMode({ width, height }), windowName)
		{

			assets = UAsset();
			assets.LoadFromFile(assetFilePath);

			renderWindow.setFramerateLimit(framerateLimit);
			ImGui::SFML::Init(renderWindow);

			font = sf::Font(fontPath); 
		}

		template<typename T>
		void AddScene()
		{
			scenes.push_back(std::make_unique<T>(*this));
		}
		void Start();
		void Update();

	private:

		const unsigned int width, height;
		bool bShowGrid;
		uei::UFloat2D gridSize;
		sf::Font font;
		sf::RenderWindow renderWindow;

		uei::UAsset assets;
		uei::UScene* currentScene;
		std::vector<std::unique_ptr<uei::UScene>> scenes;

		void TryShowGrid();
		void ShowGrid();
		void DrawLine(const uei::UFloat2D& p1, const uei::UFloat2D& p2);
		};
}
