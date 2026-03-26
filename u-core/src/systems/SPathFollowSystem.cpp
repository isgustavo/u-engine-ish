#include "SPathFollowSystem.h"
#include "UEngine.h"
#include "components/CAgent.h"
#include "components/CPath.h"
#include "components/CMovement.h"
#include "components/CTransform.h"

namespace uei
{
	SPathFollowSystem::SPathFollowSystem()
	{
		bDraw = true;
		paths.clear();
	}

	SPathFollowSystem::~SPathFollowSystem()
	{
		paths.clear();
	}

	void uei::SPathFollowSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		if (bDraw)
		{
			paths.clear();
		}

		for (auto& e : entities)
		{
			auto* cAgent = e->GetComponent<CAgent>();
			if (cAgent == nullptr) continue;

			auto* cPath = e->GetComponent<CPath>();
			if (cPath == nullptr) continue;

			if (cPath->GetCurrentPathNodeIndex() < 0)
				continue;

			auto* cMovement = e->GetComponent<CMovement>();
			if (cMovement == nullptr) continue;

			auto* cTransform = e->GetComponent<CTransform>();
			if (cTransform == nullptr) continue;

			sf::Vector2f& currentPosition = cTransform->GetPosition();
			sf::Vector2i currentNode = PositionToGrid(currentPosition, engine.CurrentScene()->GridSize());
			sf::Vector2i& currentPathNode = cPath->GetCurrentPathNode();	

			if(currentNode.x == currentPathNode.x && currentNode.y == currentPathNode.y)
			{
				cPath->UpdateCurrentPathNodeIndex();
				bool isComplete = cPath->GetCurrentPathNodeIndex() < 0;
				if (!isComplete)
				{
					sf::Vector2i& newPathNode = cPath->GetCurrentPathNode();
					sf::Vector2i delta = newPathNode - currentPathNode;
					cMovement->SetCurrentMovement(VectorToMovement(delta));
				}
				else
				{
					cMovement->SetCurrentMovement(EMovement::NONE);
				}
			}

			if (bDraw)
			{
				std::vector<sf::Vector2i> path;
				for (int i = cPath->GetCurrentPathNodeIndex() - 1; i >= 0; i--)
				{
					path.push_back(cPath->GetPath()[i]);
				}
				paths.push_back(path);
			}
		}
	}

	void SPathFollowSystem::Draw(UEngine& engine)
	{
		if (bDraw)
		{
			int gridHalfSize = engine.CurrentScene()->GridSize() * .5f;
			for (int i = 0; i < paths.size(); i++)
			{
				std::vector<sf::Vector2i> path = paths[i];
				for (int j = path.size() - 1; j > 0; j--)
				{
					sf::Vector2f a = PositionToCenter(sf::Vector2f(path[j].x, path[j].y), engine.CurrentScene()->GridSize());
					sf::Vector2f b = PositionToCenter(sf::Vector2f(path[j - 1].x, path[j - 1].y), engine.CurrentScene()->GridSize());

					float ax = a.x + gridHalfSize;
					float ay = a.y + gridHalfSize;
					float bx = b.x + gridHalfSize;
					float by = b.y + gridHalfSize;

					sf::Vertex line[] = { {sf::Vector2f(ax, ay), sf::Color::Red}, {sf::Vector2f(bx, by), sf::Color::Red} };
					engine.RenderWindow().draw(line, 2, sf::PrimitiveType::Lines);
				}
			}
		}
	}
}


