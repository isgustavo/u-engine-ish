#include "SUpdateNavGridSystem.h"
#include "UScene.h"
#include <cmath>
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

			sf::Vector2i p = cTransform->GetPosition();

			int eColumns = (int)p.x / uei::GridSize;
			int eRows = (int)p.y / uei::GridSize;

			int index = eRows * uei::GridColumns + eColumns;

			int column = index % uei::GridColumns;
			int row = index / uei::GridColumns;

			int startColumn = column;
			int startRow = row;

			for (int i = startColumn; i < startColumn + cNavGridModifier->GetColumns(); i++)
			{
				for (int j = startRow; j < startRow + cNavGridModifier->GetRows(); j++)
				{
					//if ((j * engine.CurrentScene()->GridColumns() + i) < 0) continue;
					if ((j * uei::GridColumns) + i >= engine.CurrentScene()->GetNavGrid().size()) break;
					engine.CurrentScene()->GetNavGrid()[(j * uei::GridColumns) + i] += cNavGridModifier->GetWeight();
				}
			}
		}

		engine.CurrentScene()->SetNavGridDirty(false);
	}

	void SUpdateNavGridSystem::Draw(UEngine& engine)
	{
		if(!engine.CurrentScene()->ShowNavGrid()) return;

		for (int i = 0; i < uei::GridColumns; i++)
		{
			for (int j = 0; j < uei::GridRows; j++)
			{
				sf::RectangleShape grid(sf::Vector2f(uei::GridSize, uei::GridSize));

				sf::Vector2i t = sf::Vector2i(i, j);
				sf::Vector2f tt = sf::Vector2f(GridToPosition(t, uei::GridSize).x, GridToPosition(t, uei::GridSize).y);
				grid.setPosition(tt);

				if (engine.CurrentScene()->GetNavGrid()[(j * uei::GridColumns) + i] == 0)
				{
					grid.setFillColor(sf::Color(0, 0, 255, 0));
				}
				else
				{
					grid.setFillColor(sf::Color(0, 0, 255, 200));
				}

				engine.RenderWindow().draw(grid);
			}
		}
	}
}