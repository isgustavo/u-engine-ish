#include "UScene.h"
#include "entities/UEntity.h"

#include <string>
#include <memory>
#include <iostream>
#include <components/CTransform.h>
#include <components/CObstacle.h>
#include <components/CNavGridModifier.h>

namespace uei
{
	UScene::UScene(UEngine& inEngine) :
		engine(inEngine),
		navGrid(engine.Rows() * engine.Columns(), -1),
		navGridColumnSize(engine.Columns()),
		navGridSqrSize(engine.GridSize().x),
		entities(), entitiesMap(), toAdd(),
		bIsPause(false)
	{
	}
	UEntity& UScene::AddEntity(const std::string& inTag)
	{
		toAdd.push_back(std::unique_ptr<UEntity>(new UEntity(entities.size(), inTag)));
		return *toAdd.back();
	}
	void UScene::Start()
	{
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
	void UScene::UpdateNavGrid()
	{
		std::cout << "Start UpdateNavGrid" << navGrid.size() << std::endl;
		for (int i = 0; i < navGrid.size(); i++)
		{
			navGrid[i] = 0;
		}

		for (auto& e : entities)
		{
			auto c_transform = e.get()->GetComponent<uei::CTransform>();
			auto c_navGridModifier = e.get()->GetComponent<uei::CNavGridModifier>();

			if (c_navGridModifier == nullptr || c_transform == nullptr) continue;

			int eColumns = (int)c_transform->Position().x / engine.GridSize().x;
			int eRows = (int)c_transform->Position().y / engine.GridSize().y;

			for (int i = eColumns; i < eColumns + c_navGridModifier->Columns(); i++)
			{
				for (int j = eRows; j < eRows + c_navGridModifier->Rows(); j++)
				{
					navGrid[i * engine.Columns() + j] += c_navGridModifier->Weight();
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
		OnDraw();
	}
}