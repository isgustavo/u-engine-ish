#pragma once

#include "entities/UEntity.h"
#include "UEngine.h"
#include <vector>
#include <memory>
#include <map>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>
#include <systems/USystem.h>

namespace uei
{
	class UEngine;

	class UScene
	{
		friend class UEngine;

	public:
		UScene(UEngine& inEngine);

		UEntity& AddEntity(std::string& inTag);
		
		template<typename T, typename... Args>
		void AddSystem(Args&&... args)
		{
			static_assert(std::is_base_of_v<uei::USystem, T>,
				"T must derive from USystem");

			systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		}
		
		//uei::UEngine& Engine();
		std::vector<std::unique_ptr<uei::UEntity>>& Entities() { return entities; }
		std::unordered_map<std::string, std::unique_ptr<uei::UEntity>>& Prefabs() { return prefabs; }
		//std::vector<std::unique_ptr<uei::USystem>>& Entities() { return entities; }
		//const std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(const std::string& inTag);

		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;
		virtual void Editor_Show() = 0;
		void SetNavGridDirty() { bIsNavGridDirty = true; }

	protected:
		UEngine& engine;

		std::unordered_map<std::string, std::unique_ptr<uei::UEntity>> prefabs;

		std::vector<std::unique_ptr<uei::UEntity>> entities;
		std::vector<std::unique_ptr<uei::USystem>> systems;
		std::vector<int> navGrid;

	private:	
		std::string levelName;
		int gridColumns;
		int gridRows;
		int gridSize;

		std::map<std::string, std::vector<uei::UEntity*>> entitiesMap;
		std::vector<std::unique_ptr<uei::UEntity>> toAdd;

		bool bIsPause;
		bool bIsNavGridDirty = false;

		void Start(std::string levelName);
		void Update();
		void UpdateNavGrid();
		void AddNewEntities();
		void RemoveInactiveEntities();
		void Draw();
		void DrawGrid();
		void DrawLine(const sf::Vector2f& p1, const sf::Vector2f& p2);
	};
}
