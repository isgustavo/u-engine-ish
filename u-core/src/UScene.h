#pragma once

#include "entities/UEntity.h"
#include <vector>
#include <memory>
#include <map>

namespace uei
{
	class UEngine;

	class UScene
	{

		friend UEngine;

	public:
		UScene(const uei::UEngine& inEngine) :
			engine(&inEngine), 
			entities(), entitiesMap(), toAdd(),
			bIsPause(false)
		{

		}

		template<typename T>
		UEntity& AddEntity(const std::string& inTag)
		{
			toAdd.push_back(std::make_unique<T>(entities.size(), inTag));
			return *toAdd.back();
		}

		//const std::vector<std::shared_ptr<Entity>>& GetEntities();
		//const std::vector<std::shared_ptr<Entity>>& GetEntitiesByTag(const std::string& inTag);

		virtual void OnStart() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDraw() = 0;

	private:
		const uei::UEngine* engine;
		
		std::vector<std::unique_ptr<uei::UEntity>> entities;
		std::map<std::string, std::vector<uei::UEntity*>> entitiesMap;
		std::vector<std::unique_ptr<uei::UEntity>> toAdd;

		bool bIsPause;

		void Start();
		void Update();
		void AddNewEntities();
		void RemoveInactiveEntities();
		void Draw();
	};
}
