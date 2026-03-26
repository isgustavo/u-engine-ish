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

namespace uei
{
	class UEngine;
	class USystem;

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
		virtual void OnUpdate(float deltaTime) = 0;
		//virtual void OnFixedUpdate(float fixedDeltaTime) = 0;
		virtual void OnDraw() = 0;
		virtual void OnMouseLeft() = 0;
		virtual void OnMouseRight() = 0;

		sf::View& View() { return view; }
		int GridColumns() const { return gridColumns; }
		int GridRows() const { return gridRows; }
		int GridSize() const { return gridSize; }

		std::vector<int>& GetNavGrid() { return navGrid; }
		bool ShowNavGrid() { return bShowNavGrid; }
		bool GetNavGridDirty() { return bIsNavGridDirty; }
		void SetNavGridDirty(bool value) { bIsNavGridDirty = value; }

	protected:
		UEngine& engine;
		sf::View view;

		std::vector<UEntity*> entities;
		std::vector<USystem*> systems;
		std::vector<int> navGrid;

		void AddEntity(UEntity* entity, const sf::Vector2f& position);
		void AddEntity(UEntity* entity);

		int gridColumns;
		int gridRows;
		int gridSize;
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
