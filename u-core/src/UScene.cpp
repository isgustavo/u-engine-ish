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
	UScene::UScene(UEngine& inEngine) :
		engine(inEngine),
		navGrid(), entities(), entitiesMap(), toAdd(), bIsPause(false)
	{ 
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
		//prefabs.clear();
		entities.clear();
		systems.clear();
		for (auto& [key, value] : entitiesMap)
		{
			value.clear();
		}         
		entitiesMap.clear();
	}

	void UScene::Start(std::string levelName)
	{
		if (!levelName.empty())
		{
			Load(levelName);
		}

		OnStart();
	}
	void UScene::Restart(std::string levelName)
	{
		ClearScene();
		Start(levelName);
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
				navGrid = std::vector(gridColumn * gridRow, -1);
			}
			//else if (str == PREFAB)
			//{
			//	if (newPrefab != nullptr)
			//	{
			//		prefabs.emplace(newPrefab->name, newPrefab->Clone());
			//		newPrefab = nullptr;
			//	}
			//	std::string prefabName;
			//	file >> prefabName;

			//	newPrefab = new UEntity(0, prefabName);
			//}
			//else if (str == COMPONENT)
			//{
			//	std::string componentName;
			//	file >> componentName;
			//	if (componentName.empty()) continue;
			//	auto* newComponent = UEditor::Create(componentName);
			//	newComponent->LoadComponent(engine, file);
			//	newPrefab->AddComponent(newComponent);
			//}
			else if (str == ENTITY)
			{
				//if (newPrefab != nullptr)
				//{
				//	prefabs.emplace(newPrefab->name, newPrefab->Clone());
				//	newPrefab = nullptr;
				//}
				std::string entityName;
				float px, py;

				file >> entityName >> px >> py;
				AddEntity(entityName, sf::Vector2f(px, py));
			}

			str = "";
		}
		//if (newPrefab != nullptr)
		//{
		//	prefabs.emplace(newPrefab->name, newPrefab->Clone());
		//}

		delete newPrefab;
		newPrefab = nullptr;
	}
	void UScene::Update()
	{
		AddNewEntities();
		RemoveInactiveEntities();
		if (bIsNavGridDirty)
			UpdateNavGrid();
		OnUpdate();
	}

	void UScene::Draw()
	{
		//DrawGrid();
		OnDraw();
	}

	UEntity* UScene::AddEntity(std::string& name)
	{
		int index = entities.size();
		std::string entityName = name;// +"_" + std::to_string(index);
		toAdd.push_back(new UEntity(index, entityName));
		return toAdd.back();
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
			uei::CTransform* c_transform = e.get()->GetComponent<uei::CTransform>();
			uei::CNavGridModifier* c_navGridModifier = e.get()->GetComponent<uei::CNavGridModifier>();

			if (c_navGridModifier == nullptr || c_transform == nullptr) continue;

			int eColumns = (int)c_transform->Position().x / gridSize;
			int eRows = (int)c_transform->Position().y / gridSize;

			for (int i = eColumns; i < eColumns + c_navGridModifier->Columns(); i++)
			{
				for (int j = eRows; j < eRows + c_navGridModifier->Rows(); j++)
				{
					navGrid[i * gridColumn + j] += c_navGridModifier->Weight();
				}
			}
		}
		bIsNavGridDirty = false;
		for (int i = 0; i < navGrid.size(); i++)
		{
			std::cout << navGrid[i] << ",";
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
	}

	void UScene::DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		sf::Vertex line[] = { {p1, sf::Color::White}, {p2, sf::Color::White} };
		engine.RenderWindow().draw(line, 2, sf::PrimitiveType::Lines); // ToDo change to viewport
	}

	UEntity* UScene::AddEntity(std::string entityName, sf::Vector2f position)
	{
		auto it = engine.Assets()->Prefabs().find(entityName);
		UEntity* prefab = it->second;
		
		UEntity* newEntity = AddEntity(entityName);
		newEntity->AddComponent<CTransform>(position);

		if (prefab->HasComponent<CSprite>())
		{
			CSprite* sprite = prefab->GetComponent<CSprite>();
			CSprite* spriteCloned = (CSprite*)sprite->Clone();
			spriteCloned->SetScale(gridSize);
			newEntity->AddComponent(spriteCloned);
		}
		return newEntity;
	}
}