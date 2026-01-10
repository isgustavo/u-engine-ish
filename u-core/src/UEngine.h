#pragma once
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
			sf::Vector2f inGridSize, const std::string& fontPath, const std::string& assetFilePath)
			: screenSize(sf::Vector2i(inWidth, inHeight)), bShowGrid(false), gridSize(inGridSize), renderWindow(sf::VideoMode({ inWidth, inHeight }), windowName)
		{
			rows = (size_t)(ScreenSize().x / GridSize().x);
			columns = (size_t)(ScreenSize().y / GridSize().y);

			assets = std::make_unique<UAsset>();
			assets->LoadFromFile(assetFilePath);

			renderWindow.setFramerateLimit(framerateLimit);
			ImGui::SFML::Init(renderWindow);

			font = sf::Font(fontPath); 
		}

		template<typename T>
		void AddScene()
		{
			scenes.push_back(std::make_unique<T>(*this));
		}

		uei::UAsset& Assets();
		sf::RenderWindow& RenderWindow();
		sf::Vector2i& ScreenSize();
		sf::Vector2f& GridSize();
		size_t Rows() { return rows; }
		size_t Columns() { return columns; }

		bool ShouldShowGrid() { return bShowTarget; }

		void Start();
		void Update();

	private:

		sf::Vector2i screenSize;
		size_t rows;
		size_t columns;
		bool bShowGrid;
		bool bShowTarget;
		sf::Vector2f gridSize;
		sf::Font font;
		sf::RenderWindow renderWindow;

		std::unique_ptr<uei::UAsset> assets;
		uei::UScene* currentScene;
		std::vector<std::unique_ptr<uei::UScene>> scenes;

		void TryShowGrid();
		void ShowGrid();
		void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);
		};
}
