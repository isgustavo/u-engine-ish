#include "SUpdateNavGridSystem.h"
#include "UScene.h"
#include <components/CTransform.h>
#include <components/CNavGridModifier.h>
#include <SFML/Graphics/RectangleShape.hpp>

namespace uei
{
	SUpdateNavGridSystem::SUpdateNavGridSystem()
	{

	}

	void SUpdateNavGridSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		if (!engine.CurrentScene()->GetNavGridDirty()) return;

		for (int i = 0; i < engine.CurrentScene()->GetNavGrid().size(); i++)
		{
			engine.CurrentScene()->GetNavGrid()[i] = 0;
		}

		for (auto& e : entities)
		{
			CTransform* cTransform = e->GetComponent<CTransform>();
			CNavGridModifier* cNavGridModifier = e->GetComponent<CNavGridModifier>();

			if (cNavGridModifier == nullptr || cTransform == nullptr) continue;

			int eColumns = (int)cTransform->GetPosition().x / engine.CurrentScene()->GridSize();
			int eRows = (int)cTransform->GetPosition().y / engine.CurrentScene()->GridSize();

			int index = eRows * engine.CurrentScene()->GridColumns() + eColumns;

			int column = index % engine.CurrentScene()->GridColumns();
			int row = index / engine.CurrentScene()->GridColumns();

			int startColumn = std::max(column - cNavGridModifier->GetStartColumn(), 0);
			int startRow = std::max(row - cNavGridModifier->GetStartRow(), 0);
			for (int i = startColumn; i < startColumn + cNavGridModifier->GetColumns(); i++)
			{
				for (int j = startRow; j < startRow + cNavGridModifier->GetRows(); j++)
				{
					if ((j * engine.CurrentScene()->GridColumns()) + i >= engine.CurrentScene()->GetNavGrid().size()) break;
					engine.CurrentScene()->GetNavGrid()[(j * engine.CurrentScene()->GridColumns()) + i] += cNavGridModifier->GetWeight();
				}
			}
		}

		engine.CurrentScene()->SetNavGridDirty(false);
	}

	void SUpdateNavGridSystem::Draw(UEngine& engine)
	{
		if(!engine.CurrentScene()->ShowNavGrid()) return;

		for (int i = 0; i < engine.CurrentScene()->GridColumns(); i++)
		{
			for (int j = 0; j < engine.CurrentScene()->GridRows(); j++)
			{
				sf::RectangleShape grid(sf::Vector2f(engine.CurrentScene()->GridSize(), engine.CurrentScene()->GridSize()));

				grid.setPosition(PositionToCenter(sf::Vector2f(i, j), engine.CurrentScene()->GridSize()));

				if (engine.CurrentScene()->GetNavGrid()[(j * engine.CurrentScene()->GridColumns()) + i] == 0)
				{
					grid.setFillColor(sf::Color(0, 0, 255, 0));
				}
				else
				{
					grid.setFillColor(sf::Color(0, 0, 255, 100 + engine.CurrentScene()->GetNavGrid()[(j * engine.CurrentScene()->GridColumns()) + i]));
				}

				engine.RenderWindow().draw(grid);
			}
		}
	}
}