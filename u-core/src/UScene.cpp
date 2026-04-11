#include "UScene.h"
#include "editor/UEditorScene.h"
#include "editor/UEditor.h"
#include "entities/UEntity.h"
#include "components/CTransform.h"
#include "components/CObstacle.h"
#include "components/CNavGridModifier.h"
#include "UEngine.cpp"

#include <string>
#include <memory>
#include <iostream>

#include <filesystem>
#include <fstream>

#include <SFML/Window/Event.hpp>

namespace uei
{
	int GridColumns = 0;
	int GridRows = 0;
	int GridSize = 0;
	int GridHalfSize = 0;

	UScene::UScene(UEngine& inEngine, std::string levelName) : engine(inEngine), navGrid(), entities(), /*entitiesMap(),*/ toAdd(), bIsPause(false)
	{ 
		view = sf::View();
		view.setSize(engine.GetScreenSize());
		if (levelName == EMPTY) return;
		Load(levelName);
	}

	UScene::~UScene() 
	{
		ClearScene();
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
				file >> GridColumns >> GridRows >> GridSize;
				GridHalfSize = GridSize * 0.5f;
				navGrid = std::vector(GridColumns * GridRows, 0);
			}
			else if (str == ENTITY)
			{
				std::string entityName;
				float px, py;

				file >> entityName >> px >> py;
				UEntity* entity = engine.Assets()->GetPrefab(entityName);
				if (entity == nullptr) continue;
				sf::Vector2i gridPosition = sf::Vector2i(px, py);
				sf::Vector2i position = GridToPosition(gridPosition, uei::GridSize);
				AddEntity(entity->Clone(), position);
			}
			else if (str == SYSTEM)
			{
				std::string systemName;
				
				file >> systemName;
				USystem* newSystem = UEditor::CreateSystem(systemName);
				AddSystem(newSystem);
			}
			else if (str == PLAYER_INPUT)
			{
				int inputKey;
				std::string inputName;

				file >> inputKey >> inputName;
				inputKeys[inputKey] = inputName;
			}
			str = "";
		}

		delete newPrefab;
		newPrefab = nullptr;
		//view.setCenter({ (engine.GetScreenSize().x * 0.5f) - gridSize * 0.5f, (engine.GetScreenSize().y * 0.5f) - gridSize * 0.5f });
		view.setCenter({ (engine.GetScreenSize().x * 0.5f), (engine.GetScreenSize().y * 0.5f) });
		bIsLoaded = true;
	}

	void UScene::ClearScene()
	{
		bIsNavGridDirty = false;
		bShowNavGrid = false;
		bIsLoaded = false;
		navGrid.clear();
		entities.clear();
		systems.clear();
	}

	void UScene::Start()
	{
		OnStart();
		bIsStarted = true;
	}

	void UScene::Restart(std::string levelName)
	{
		ClearScene();
		Load(levelName);
		Start();
	}

	void UScene::Input()
	{
		while (const auto event = engine.RenderWindow().pollEvent())
		{
			ImGui::SFML::ProcessEvent(engine.RenderWindow(), *event);

			if (event->is<sf::Event::Closed>())
			{
				engine.RenderWindow().close();
			}
			
			OnInput(event);

			/*if (event->is<sf::Event::MouseButtonPressed>())
			{
				if (!ImGui::GetIO().WantCaptureMouse && currentScene != nullptr)
				{
					const auto& mouse = event->getIf<sf::Event::MouseButtonPressed>();

					if (mouse->button == sf::Mouse::Button::Left)
					{
						currentScene->OnMouseLeft();
					}
					if (mouse->button == sf::Mouse::Button::Right)
					{
						currentScene->OnMouseRight();
					}
				}
			}*/
		}
	}

	void UScene::Update()
	{
		AddNewEntities();
		RemoveInactiveEntities();

		//if (bIsNavGridDirty)
		//	UpdateNavGrid();

		for (auto* s : systems)
		{
			bool canUpdate = !IsEditorScene() || s->UpdateEditorScene();
			if (!canUpdate) continue;
			
			s->Update(engine, entities);
		}

		OnUpdate(engine.DeltaTime());
	}

	void UScene::Draw()
	{
		for (auto* s : systems)
		{
			bool canDraw = !IsEditorScene() || s->DrawEditorScene();
			if (!canDraw) continue;

			s->Draw(engine);
		}

		if (bShowNavGrid)
			DrawGrid();

		OnDraw();
	}

	void UScene::AddEntity(UEntity* entity)
	{
		int index = entities.size();
		entity->SetID(index);
		toAdd.push_back(entity);
	}

	void UScene::AddEntity(UEntity* entity, const sf::Vector2i& position)
	{
		AddEntity(entity);
		CTransform* transform = entity->GetComponent<CTransform>();
		transform->SetPosition(position);
	}

	//void UScene::AddEntity(UEntity* entity, const sf::Vector2i& position)
	//{
	//	//sf::Vector2f p = sf::Vector2f(position.x, position.y);
	//	AddEntity(entity, position);
	//}

	void UScene::AddNewEntities()
	{
		for (auto* e : toAdd)
		{
			for (auto& [key, value] : e->components)
			{
				if (e->HasComponent<CNavGridModifier>())
				{
					bIsNavGridDirty = true;
				}
				value->Start(engine);
			}

			entities.push_back(e);
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
	}

	void UScene::DrawGrid()
	{
		for (int x = 0; x < GridColumns; x++)
		{
			//DrawLine(sf::Vector2f(x * gridSize - gridSize * 0.5f, gridSize * 0.5f), sf::Vector2f(x * gridSize - gridSize * 0.5f, gridSize * gridRows - gridSize * 0.5f));
			//DrawLine(sf::Vector2f(x * gridSize - gridSize * 0.5f, -gridSize * 0.5f), sf::Vector2f(x * gridSize - gridSize * 0.5f, gridSize * gridRows));
			DrawLine(sf::Vector2f(x * GridSize, 0), sf::Vector2f(x * GridSize, GridSize * GridRows));
		}

		for (int y = 0; y < GridRows; y++)
		{
			//DrawLine(sf::Vector2f(gridSize * 0.5f, y * gridSize - gridSize * 0.5f), sf::Vector2f(gridSize * gridColumns - gridSize * 0.5f, y * gridSize - gridSize * 0.5f));
			//DrawLine(sf::Vector2f(-gridSize * 0.5f, y * gridSize - gridSize * 0.5f), sf::Vector2f(gridSize * gridColumns - gridSize * 0.5f, y * gridSize - gridSize * 0.5f));
			DrawLine(sf::Vector2f(0, y * GridSize), sf::Vector2f(GridSize * GridColumns, y * GridSize));
		}

		sf::Text gridText(engine.Assets()->GetDefaultFont(), "", 8);
		for (int x = 0; x < GridColumns; x++)
		{
			for (int y = 0; y < GridRows; y++)
			{
				std::string xGrid = std::to_string(x);
				std::string yGrid = std::to_string(y);
				
				gridText.setString("(" + xGrid + "," + yGrid + ")");
								
				//gridText.setPosition(sf::Vector2f(x * gridSize - gridSize * 0.5f, y * gridSize - gridSize * 0.5f));
				gridText.setPosition(sf::Vector2f(x * GridSize, y * GridSize));

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