#include "UScene.h"
#include "entities/UEntity.h"

#include <string>
#include <memory>
#include <iostream>
#include <components/CTransform.h>
#include <components/CObstacle.h>
#include <components/CNavGridModifier.h>
#include "editor/Editor_ComponentRegistry.h"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace uei
{
	UScene::UScene(UEngine& inEngine) :
		engine(inEngine),
		navGrid(), entities(), entitiesMap(), toAdd(),
		bIsPause(false) 
	{ 
	}

	void UScene::Start(std::string levelName)
	{

		//const std::string LEVEL("Level");
		//const std::string PREFAB("Prefab");
		//const std::string COMPONENT("Component");
		const std::string LEVEL_DESIGN("levelDesign");

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
			if (str == LEVEL)
			{
				file >> gridColumns >> gridRows >> gridSize;
				navGrid = std::vector(gridColumns * gridRows, -1);
			}
			else if (str == PREFAB)
			{
				if (newPrefab != nullptr)
				{
					prefabs.emplace(newPrefab->name, newPrefab->Clone());
					newPrefab = nullptr;
				}
				std::string prefabName;
				file >> prefabName;

				newPrefab = new UEntity(0, prefabName);
			}
			else if (str == COMPONENT)
			{
				std::string componentName;
				file >> componentName;
				if (componentName.empty()) continue;
				auto* newComponent = Editor_ComponentRegistry::Create(componentName);
				newComponent->Editor_Load(file);
				newPrefab->AddComponent(newComponent);
			}
			else if (str == LEVEL_DESIGN)
			{
				//std::string prefabName;
				//float x, y;
				//file >> prefabName >> x >> y;

				//auto it = prefabs.find(prefabName);
				//assert(it != prefabs.end());
				//std::unique_ptr<UEntity> newEntity = it->second->Clone();
				//CTransform* transform = newEntity->GetComponent<CTransform>();
				//transform->SetPosition(sf::Vector2f(x, y));
			}
		}

		if (newPrefab != nullptr)
		{
			prefabs.emplace(newPrefab->name, newPrefab->Clone());
		}
		
		delete newPrefab;
		newPrefab = nullptr;

		OnStart();
	}
	void UScene::Update()
	{
		AddNewEntities();
		RemoveInactiveEntities();
		if (bIsNavGridDirty)
			UpdateNavGrid();
		OnUpdate();
	}
	UEntity& UScene::AddEntity(std::string& inTag)
	{
		toAdd.push_back(std::unique_ptr<UEntity>(new UEntity(entities.size(), inTag)));
		return *toAdd.back();
	}
	void UScene::AddNewEntities()
	{
		for (auto& e : toAdd)
		{
			auto tag = e->Name();
			entities.push_back(std::move(e));
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
			uei::CTransform* c_transform = e.get()->GetComponent<uei::CTransform>();
			uei::CNavGridModifier* c_navGridModifier = e.get()->GetComponent<uei::CNavGridModifier>();

			if (c_navGridModifier == nullptr || c_transform == nullptr) continue;

			int eColumns = (int)c_transform->Position().x / gridSize;
			int eRows = (int)c_transform->Position().y / gridSize;

			for (int i = eColumns; i < eColumns + c_navGridModifier->Columns(); i++)
			{
				for (int j = eRows; j < eRows + c_navGridModifier->Rows(); j++)
				{
					navGrid[i * gridColumns + j] += c_navGridModifier->Weight();
				}
			}
		}
		bIsNavGridDirty = false;
		for (int i = 0; i < navGrid.size(); i++)
		{
			std::cout << navGrid[i] << ",";
		}
	}

	void UScene::Draw()
	{
		DrawGrid();
		OnDraw();
	}

	void UScene::DrawGrid()
	{
		for (int x = 0; x < gridColumns; x++)
		{
			DrawLine(sf::Vector2f(x * gridSize, 0), sf::Vector2f(x * gridSize, gridSize * gridRows));
		}

		for (int y = 0; y < gridRows; y++)
		{
			DrawLine(sf::Vector2f(0, y * gridSize), sf::Vector2f(gridSize * gridColumns, y * gridSize));
		}
	}

	void UScene::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		sf::Vertex line[] = { {p1, sf::Color::White}, {p2, sf::Color::White} };
		engine.RenderWindow().draw(line, 2, sf::PrimitiveType::Lines); // ToDo change to viewport
	}

}