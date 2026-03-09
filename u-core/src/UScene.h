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

		template<typename T, typename... Args>
		void AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<uei::USystem, T>, "T must derive from USystem");

			systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		}

		template<typename T, typename... Args>
		void AddDrawSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<uei::USystem, T>, "T must derive from USystem");

			drawSystems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		}
		
		std::vector<std::unique_ptr<uei::UEntity>>& Entities() { return entities; }

		virtual void OnStart() = 0;
		virtual void OnUpdate(float deltaTime) = 0;
		//virtual void OnFixedUpdate(float fixedDeltaTime) = 0;
		virtual void OnDraw() = 0;
		virtual void OnMouseLeft() = 0;
		virtual void OnMouseRight() = 0;

		sf::View& View() { return view; }
		int GridColumn() { return gridColumn; }
		int GridRow() { return gridRow; }
		int GridSize() { return gridSize; }

		void SetNavGridDirty() { bIsNavGridDirty = true; }

	protected:
		UEngine& engine;
		sf::View view;

		std::vector<std::unique_ptr<uei::UEntity>> entities;
		std::vector<std::unique_ptr<uei::USystem>> systems;
		std::vector<std::unique_ptr<uei::USystem>> drawSystems;
		std::vector<int> navGrid;

		void AddEntity(UEntity* entity, sf::Vector2f position);
		void AddEntity(UEntity* entity);

		int gridColumn;
		int gridRow;
		int gridSize;

		void Restart(std::string levelName);
		void ClearScene();
		void DrawGrid();

	private:	

		std::string levelName;

		std::map<std::string, std::vector<uei::UEntity*>> entitiesMap;
		std::vector<uei::UEntity*> toAdd;

		bool bIsPause;
		bool bIsStarted = false;
		bool bIsNavGridDirty = false;

		void Start();
		void Load(std::string levelName);
		void Update();
		//void FixedUpdate(float fixedDeltaTime);
		void UpdateNavGrid();
		void AddNewEntities();
		void RemoveInactiveEntities();
		void Draw();
		void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);
	};
}
