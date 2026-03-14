#include "UScene.h"
#include "entities/UEntity.h"

#include <string>
#include <memory>
#include <iostream>
#include <components/CTransform.h>
#include <components/CObstacle.h>
#include <components/CNavGridModifier.h>

#include <filesystem>
#include <fstream>
#include <UEngine.cpp>

namespace uei
{
	UScene::UScene(UEngine& inEngine, std::string levelName) :
		engine(inEngine),
		navGrid(), entities(), entitiesMap(), toAdd(), bIsPause(false)
	{ 
		if (levelName != EMPTY)
		{
			Load(levelName);
		}

		view = sf::View();
		view.setSize(engine.ScreenSize());
		view.setCenter({ engine.ScreenSize().x * 0.5f, engine.ScreenSize().y * 0.5f });
	}

	UScene::~UScene() 
	{
		ClearScene();
	}

	void UScene::ClearScene()
	{
		bIsNavGridDirty = false;
		bShowNavGrid = false;
		navGrid.clear();
		entities.clear();
		systems.clear();
		for (auto& [key, value] : entitiesMap)
		{
			value.clear();
		}         
		entitiesMap.clear();
	}

	void UScene::Start()
	{
		OnStart();
		bIsNavGridDirty = true;
		bIsStarted = true;
	}
	void UScene::Restart(std::string levelName)
	{
		ClearScene();
		Load(levelName);
		Start();
	}
	void UScene::Load(std::string levelName)
	{
		std::string levelPath = "Assets/" + levelName + ".txt";

		if (!std::filesystem::exists(levelPath))
		{
			std::cerr << "File not found:" + levelPath << std::endl;
			return;
		}

		std::ifstream file(levelPath);
		if (!file)
		{
			std::cerr << "Failed to open file: " + levelPath << std::endl;
			return;
		}

		std::string str;
		uei::UEntity* newPrefab = nullptr;

		while (file.good())
		{
			file >> str;
			
			if (str.empty()) break;

			if (str == LEVEL)
			{
				file >> gridColumn >> gridRow >> gridSize;
				navGrid = std::vector(gridColumn * gridRow, 0);
			}
			else if (str == ENTITY)
			{
				std::string entityName;
				float px, py;

				file >> entityName >> px >> py;
				uei::UEntity entity = engine.Assets()->GetPrefab(entityName);
				AddEntity(entity.Clone(), sf::Vector2f(px, py));
			}

			str = "";
		}

		delete newPrefab;
		newPrefab = nullptr;
	}
	
	void uei::UScene::Update()
	{
		AddNewEntities();
		RemoveInactiveEntities();

		if (bIsNavGridDirty)
			UpdateNavGrid();

		for (auto& s : systems)
		{
			s.get()->Update(engine, entities);
		}

		OnUpdate(engine.DeltaTime());
	}

	void UScene::Draw()
	{
		for (auto& s : drawSystems)
		{
			s.get()->Update(engine, entities);
		}

		if (bShowNavGrid)
			ShowNavGrid();

		OnDraw();
	}

	void UScene::AddEntity(UEntity* entity)
	{
		int index = entities.size();
		entity->SetID(index);
		toAdd.push_back(entity);
	}
	void UScene::AddEntity(UEntity* entity, sf::Vector2f position)
	{
		AddEntity(entity);
		CTransform* transform = entity->GetComponent<CTransform>();
		transform->InitPosition(position);
	}

	void UScene::AddNewEntities()
	{
		for (auto* e : toAdd)
		{
			auto tag = e->Name();
			entities.push_back(std::unique_ptr<UEntity>(e));
			entitiesMap[tag].push_back(entities.back().get());
		}
		toAdd.clear();
	}
	void UScene::RemoveInactiveEntities()
	{
		entities.erase(
			std::remove_if(entities.begin(), entities.end(),
				[](const auto& e)
				{
					return !e->IsActive();
				}),
			entities.end()
		);

		for (auto& [tag, entities] : entitiesMap)
		{
			entities.erase(
				std::remove_if(entities.begin(), entities.end(),
					[](const auto& e)
					{
						return e != nullptr && !e->IsActive();
					}),
				entities.end()
			);
		}
	}

	void UScene::UpdateNavGrid()
	{
		for (int i = 0; i < navGrid.size(); i++)
		{
			navGrid[i] = 0;
		}

		for (auto& e : entities)
		{
			CTransform* cTransform = e.get()->GetComponent<CTransform>();
			CNavGridModifier* cNavGridModifier = e.get()->GetComponent<CNavGridModifier>();
			
			if (cNavGridModifier == nullptr || cTransform == nullptr) continue;

			int eColumns = (int)cTransform->Position().x / gridSize;
			int eRows = (int)cTransform->Position().y / gridSize;

			int index = eRows * gridColumn + eColumns;

			int column = index % gridColumn;
			int row = index / gridColumn;

			int startColumn = std::max(column - cNavGridModifier->StartColumn(), 0);
			int startRow = std::max(row - cNavGridModifier->StartRow(), 0);
			for (int i = startColumn; i < startColumn + cNavGridModifier->Columns(); i++)
			{
				for (int j = startRow; j < startRow + cNavGridModifier->Rows(); j++)
				{
					if ((j * gridColumn + i) >= navGrid.size()) break;
					navGrid[j * gridColumn + i] += cNavGridModifier->Weight();
				}
			}
		}

		bIsNavGridDirty = false;
	}

	void UScene::ShowNavGrid()
	{
		for (int i = 0; i < gridColumn; i++)
		{
			for (int j = 0; j < gridRow; j++)
			{
				sf::RectangleShape grid(sf::Vector2f(gridSize, gridSize));
				grid.setPosition(sf::Vector2f(i * gridSize, j * gridSize));

				if (navGrid[j * gridColumn + i] == 0)
				{
					grid.setFillColor(sf::Color(0, 0, 255, 0));
				}
				else 
				{
					grid.setFillColor(sf::Color(0, 0, 255, 100 + navGrid[j * gridColumn + i]));
				}

				engine.RenderWindow().draw(grid);
			}
		}
	}

	void UScene::DrawGrid()
	{
		for (int x = 0; x < gridColumn; x++)
		{
			DrawLine(sf::Vector2f(x * gridSize, 0), sf::Vector2f(x * gridSize, gridSize * gridRow));
		}

		for (int y = 0; y < gridRow; y++)
		{
			DrawLine(sf::Vector2f(0, y * gridSize), sf::Vector2f(gridSize * gridColumn, y * gridSize));
		}

		sf::Text gridText(engine.Assets()->GetDefaultFont(), "", 8);
		for (int x = 0; x < gridColumn; x++)
		{
			for (int y = 0; y < gridRow; y++)
			{
				std::string xGrid = std::to_string(x);
				std::string yGrid = std::to_string(y);
				
				gridText.setString("(" + xGrid + "," + yGrid + ")");
				gridText.setPosition({(float)x * (gridSize), (float)y * (gridSize)});

				engine.RenderWindow().draw(gridText);
			}
		}
	}

	void UScene::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		sf::Vertex line[] = { {p1, sf::Color::White}, {p2, sf::Color::White} };
		engine.RenderWindow().draw(line, 2, sf::PrimitiveType::Lines); // ToDo change to viewport
	}


}