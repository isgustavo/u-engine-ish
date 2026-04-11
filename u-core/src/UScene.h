#pragma once

#include "entities/UEntity.h"
#include "systems/USystem.h"

#include "UEngine.h"
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>
#include <iostream>

namespace uei
{
	enum EInputState
	{
		JUST_PRESSED,
		PRESSED,
		JUST_RELEASED,
		RELEASED
	};

	class PlayerInput
	{
	public:
		EInputState GetInput(sf::Keyboard::Key key)
		{
			return inputs[key];
		}

		void SetInput(sf::Keyboard::Key key, bool isPressed)
		{
			if (inputs[key] == RELEASED && isPressed)
			{
				inputs[key] = JUST_PRESSED;
				std::cout << "JUST_PRESSED" << std::endl;
			}
			else if (inputs[key] == JUST_PRESSED && isPressed)
			{
				inputs[key] = PRESSED;
				std::cout << "PRESSED" << std::endl;
			}
			else if (inputs[key] == PRESSED && !isPressed)
			{
				inputs[key] = JUST_RELEASED;
				std::cout << "JUST_RELEASED" << std::endl;
			}
			else if (inputs[key] == JUST_RELEASED && isPressed)
			{
				inputs[key] = JUST_PRESSED;
				std::cout << "JUST_PRESSED" << std::endl;
			}
			else if (inputs[key] == JUST_RELEASED && !isPressed)
			{
				inputs[key] = RELEASED;
				std::cout << "RELEASED" << std::endl;
			}	
		}

	private:

		std::unordered_map<sf::Keyboard::Key, EInputState> inputs;
	};

	class UEngine;
	class USystem;

	extern int GridColumns;
	extern int GridRows;
	extern int GridSize;
	extern int GridHalfSize;

	class UScene
	{
		friend class UEngine;
		friend class UEditor;

	public:
		UScene(UEngine& inEngine, std::string levelName);
		~UScene();

		void AddSystem(USystem* system)
		{
			systems.push_back(system);
		}
		
		std::vector<UEntity*> Entities() { return entities; }

		virtual bool IsEditorScene() { return false; }

		virtual void OnStart() = 0;
		virtual void OnInput(const std::optional<class sf::Event> event) = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		//virtual void OnFixedUpdate(float fixedDeltaTime) = 0;
		virtual void OnDraw() = 0;
		virtual void OnMouseLeft() = 0;
		virtual void OnMouseRight() = 0;

		sf::View& View() { return view; }
		//int GridColumns() const { return gridColumns; }
		//int GridRows() const { return gridRows; }
		//int GridSize() const { return gridSize; }
		//int GridHalfSize() const { return gridHalfSize; }

		std::vector<int>& GetNavGrid() { return navGrid; }
		bool ShowNavGrid() { return bShowNavGrid; }
		bool GetNavGridDirty() { return bIsNavGridDirty; }
		void SetNavGridDirty(bool value) { bIsNavGridDirty = value; }

		PlayerInput GetPlayerInput() { return currentPlayerInput; }

	protected:
		UEngine& engine;
		sf::View view;

		std::vector<UEntity*> entities;
		std::vector<USystem*> systems;
		//char editorNameBuffer[128] = ""
		PlayerInput currentPlayerInput;
		std::unordered_map<int, std::string> inputKeys;
		std::vector<int> navGrid;

		void AddEntity(UEntity* entity, const sf::Vector2f& position);
		void AddEntity(UEntity* entity, const sf::Vector2i& position);
		void AddEntity(UEntity* entity);

		//int gridColumns;
		//int gridRows;
		//int gridSize;
		//int gridHalfSize;
		bool bShowNavGrid = true;

		void Restart(std::string levelName);
		void ClearScene();
		void DrawGrid();

	private:	

		std::string levelName;

		std::vector<uei::UEntity*> toAdd;

		bool bIsPause;
		bool bIsStarted = false;
		bool bIsLoaded = false;
		bool bIsNavGridDirty = false;
		

		void Load(std::string levelName);
		void Start();
		void Input();
		void Update();
		//void FixedUpdate(float fixedDeltaTime);
		//void UpdateNavGrid();
		//void ShowNavGrid();

		void AddNewEntities();
		void RemoveInactiveEntities();
		void Draw();
		void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);
	};
}
