#pragma once
#include "UAsset.h"
#include "UScene.h"

#include <string>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>

inline const std::string LEVEL = "Level";
inline const std::string PREFAB = "Prefab";
inline const std::string COMPONENT = "Component";

namespace uei
{
	class UEngine
	{
	public:
		UEngine(unsigned int inWidth, unsigned int inHeight, const std::string& windowName, unsigned int framerateLimit/*, 
			int inRows, int inColumns, int inGridSize, const std::string& fontPath, const std::string& assetFilePath*/, bool showEditor = true)
			: screenSize(sf::Vector2i(inWidth, inHeight)), /*bShowGrid(false), rows(inRows), columns(inColumns), gridSize(inGridSize),*/
				renderWindow(sf::VideoMode	({ inWidth, inHeight }), windowName), bShowEditor(showEditor)
		{
			assets = std::make_unique<UAsset>();
			//assets->LoadFromFile(assetFilePath);

			renderWindow.setFramerateLimit(framerateLimit);
			ImGui::SFML::Init(renderWindow);

			//gridView = sf::View(sf::FloatRect({ 0.f, 0.f }, { 512.0f, 512.0f}));
			//gridView = sf::View();// sf::FloatRect({ 0.0f, 0.0f }, { (float)inWidth, (float)inHeight }));
			//gridView.setSize({ (float)inWidth, (float)inHeight });
			//gridView.setCenter({ (float)inWidth * 0.5f, (float)inHeight * 0.5f });
			
			float windowRatio = (float) inWidth / (float)inHeight;
			float viewRatio = 1.0f;

			float sizeX = 1.f;
			float sizeY = 1.f;
			float posX = 0.f;
			float posY = 0.f;

			if (windowRatio > viewRatio)
			{
				sizeX = viewRatio / windowRatio;
				posX = (1.f - sizeX) * 0.5f;
			}
			else
			{
				sizeY = windowRatio / viewRatio;
				posY = (1.f - sizeY) * 0.5f;
			}

			std::cout << sizeX << "," << sizeY << std::endl;
			std::cout << posX << "," << posY << std::endl;

			//gridView.setViewport(sf::FloatRect({ 0.0f, 0.0f }, { 0.1f, 1.0f }));

			//renderWindow.setView(gridView);
			bShowGrid = true;
			editor_bShowNewPrefab = false;
			//font = sf::Font(fontPath); 


		}

		~UEngine();

		template<typename T>
		void AddScene()
		{
			scenes.push_back(std::make_unique<T>(*this));
		}

		uei::UAsset& Assets();
		sf::RenderWindow& RenderWindow();
		sf::Vector2i& ScreenSize();


		bool ShouldShowGrid() { return bShowTarget; }

		void LoadAsset();

		void Start();
		void Update();

	private:

		sf::Vector2i screenSize;
		int rows;
		int columns;
		bool bShowGrid;
		bool bShowEditor;
		bool bShowTarget;

		int componentSelecteditem = 0;

		sf::Font font;

		sf::RenderWindow renderWindow;

		std::unique_ptr<uei::UAsset> assets;
		uei::UScene* currentScene;
		std::vector<std::unique_ptr<uei::UScene>> scenes;

		int editor_gridColumn = 5;
		int editor_gridRow = 5;
		int editor_gridSize = 90;
		char editor_levelName[128] = "";
		char editor_prefabName[128] = "";
		uei::UEntity* editor_Prefab;

		bool editor_bLevelLoaded;
		bool editor_bNewLevel;
		bool editor_bShowNewPrefab;
		bool editor_bShowEditPrefab;

		void Editor_Show();
		void Editor_ShowFileTab();
		void Editor_ShowPrefabTab();
		void Editor_ShowPrefabGallery();
		void Editor_NewPrefab();
		void Editor_ShowNewPrefab();
		void Editor_ShowEditPrefab();
		void Editor_SavePrefab();
		void Editor_SaveUpdatePrefab();
		void Editor_LoadLevel();
		void Editor_SaveLevel();
		void Editor_CreateLevel();
	};
}