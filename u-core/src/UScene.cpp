#include "UScene.h"
#include "entities/UEntity.h"

#include <string>
#include <memory>

namespace uei
{
	UEntity& UScene::AddEntity(const std::string& inTag)
	{
		toAdd.push_back(std::unique_ptr<UEntity>(new UEntity(entities.size(), inTag)));
		return *toAdd.back();
	}
	/*UEngine& UScene::Engine()
	{
		return *engine;
	}*/
	void UScene::Start()
	{
		OnStart();
	}
	void UScene::Update()
	{
		AddNewEntities();
		RemoveInactiveEntities();
		OnUpdate();
	}
	void UScene::AddNewEntities()
	{
		for (auto& e : toAdd)
		{
			auto tag = e->GetTag();
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
	void UScene::Draw()
	{
		OnDraw();
	}
}