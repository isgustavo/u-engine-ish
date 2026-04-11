#include "SPathfinderSystem.h"

#include <entities/UEntity.h>
#include <components/CAgent.h>
#include <components/CPathRequest.h>
#include <components/CPath.h>
#include <components/CTransform.h>
#include <components/CMovementAnimation.h>
#include <components/CGridMovement.h>
#include <components/CNavGridModifier.h>
#include <components/CPlayer.h>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <UScene.h>


namespace uei
{
	Node::Node(const sf::Vector2i& inPosition, Node* inParent)
	{
		parent = inParent;
		position = inPosition;
		G = H = 0;
	}

	int Node::GetScore() const
	{
		return G + H;
	}

	SPathfinderSystem::SPathfinderSystem()
	{
		heuristic = Heuristic::Euclidean;
	}

	void SPathfinderSystem::Update(UEngine& engine, std::vector<UEntity*> entities)
	{
		for (auto& e : entities)
		{
			auto* cAgent = e->GetComponent<CAgent>();
			if (cAgent == nullptr) continue;

			auto* cPathRequest = e->GetComponent<CPathRequest>();
			if (cPathRequest == nullptr) continue;

			auto* cMovement = e->GetComponent<CGridMovement>();
			if (cMovement == nullptr) continue;

			auto* cTransform = e->GetComponent<CTransform>();
			if (cTransform == nullptr) continue;

			sf::Vector2i targetGripPosition = PositionToGrid(cPathRequest->TargetPosition(), uei::GridSize);
			sf::Vector2i targetGridPositionClamp = sf::Vector2i(std::clamp(targetGripPosition.x, 0, uei::GridColumns-1),
				std::clamp(targetGripPosition.y, 0, uei::GridRows-1));

			CPath* path = e->GetOrAddComponent<CPath>();
			path->SetPath(FindPath(engine.CurrentScene()->GetNavGrid(),
					targetGridPositionClamp,
					PositionToGrid(cTransform->GetPosition(), uei::GridSize),
					cPathRequest->GetInitialInvalidGridMovement(),
					cMovement->GetValidGridMovement(),
					1,
					1,
					uei::GridColumns));

			e->RemoveComponent<uei::CPathRequest>();
		}
	}

	std::vector<sf::Vector2i> SPathfinderSystem::FindPath(const std::vector<int>& inNavGrid,
		const sf::Vector2i& targetGridPosition, const sf::Vector2i& agentGridPosition,
		const std::vector<sf::Vector2i>& agentInitialInvalidGridMovement,
		const std::vector<sf::Vector2i>& agentValidGridMovement,
		const int agentGridColumn, const int agentGridRow, const int gridColumns)
	{

		int targetIndex = (targetGridPosition.y * gridColumns) + targetGridPosition.x;

		bool bFirstNode = true;
		Node* currentNode = nullptr;

		openVector.push_back(new Node(agentGridPosition, nullptr));
		while (!openVector.empty())
		{
			auto current_it = openVector.begin();
			currentNode = *current_it;

			for (auto it = current_it; it != openVector.end(); it++) 
			{
				auto node = *it;
				if (node->GetScore() <= currentNode->GetScore())
				{
					currentNode = node;
					current_it = it;
				}
			}

			if (currentNode->position == targetGridPosition)
			{
				break;
			}

			closeVector.push_back(currentNode);
			openVector.erase(current_it);

			for (size_t i = 0; i < agentValidGridMovement.size(); i++)
			{
				if (currentNode->parent == nullptr)
				{
					bool isInitialInvalidGridMovement = false;
					for (size_t j = 0; j < agentInitialInvalidGridMovement.size(); j++)
					{
						if (agentValidGridMovement[i] == agentInitialInvalidGridMovement[j])
						{
							isInitialInvalidGridMovement = true;
							break;
						}
					}

					if (isInitialInvalidGridMovement)
						continue;
				}

				const sf::Vector2i newCoordinate(currentNode->position + agentValidGridMovement[i]);
				if (IsOutOfNavGrid(inNavGrid, targetIndex, currentNode->position, agentValidGridMovement[i], agentGridColumn, agentGridRow, gridColumns) ||
					FindNode(closeVector, newCoordinate))
				{
					continue;
				}

				const int newCoordinateIndex = (newCoordinate.y * gridColumns) + newCoordinate.x;
				const int newCoordinateCost = currentNode->G + (((i < 4) ? 100 : 142) + inNavGrid[newCoordinateIndex]);

				Node* newCoordinateNode = FindNode(openVector, newCoordinate);
				if (newCoordinateNode == nullptr)
				{
					newCoordinateNode = new Node(newCoordinate, currentNode);
					newCoordinateNode->G = newCoordinateCost;
					newCoordinateNode->H = GetHeuristicCost(newCoordinate, targetGridPosition);
					openVector.push_back(newCoordinateNode);
				}
				else if (newCoordinateCost < newCoordinateNode->G)
				{
					newCoordinateNode->parent = currentNode;
					newCoordinateNode->G = newCoordinateCost;
				}
			}
		}

		std::vector<sf::Vector2i> path;
		while (currentNode != nullptr)
		{
			if (inNavGrid[(currentNode->position.y * gridColumns) + currentNode->position.x] < 1000)
			{
				path.push_back(currentNode->position);
			}
			currentNode = currentNode->parent;
		}

		for (auto it = openVector.begin(); it != openVector.end();) 
		{
			delete* it;
			it = openVector.erase(it);
		}
		openVector.clear();

		for (auto it = closeVector.begin(); it != closeVector.end();)
		{
			delete* it;
			it = closeVector.erase(it);
		}
		closeVector.clear();
		
		return path;
	}

	bool SPathfinderSystem::IsOutOfNavGrid(const std::vector<int>& inNavGrid, const int targetIndex, 
		const sf::Vector2i& currentNode, 
		const sf::Vector2i& nextMovement, 
		const int agentGridColumn, const int agentGridRow, const int gridColumns)
	{
		const sf::Vector2i newCoordinate(currentNode + nextMovement);
		for (int i = newCoordinate.x; i < newCoordinate.x + agentGridColumn; i++)
		{
			for (int j = newCoordinate.y; j < newCoordinate.y + agentGridRow; j++)
			{
				//if (i < 0 || i >= inGridColumn || j < 0 || j >= inGridRow)
				//{
					//std::cout << "IsOutOfNavGrid" << i << "..." << inGridColumn
					//	<< "," << inGridRow << "..." << j << std::endl;
				//	return true;
				//}

				int currentIndex = (j * gridColumns) + i;
				if (currentIndex >= inNavGrid.size())
				{
					return true;
				}

				if (inNavGrid[currentIndex] == 1000)
				{
					if (currentIndex == targetIndex) 
						return false;
					return true;
				}
			}
		}
		return false;
	}

	Node* SPathfinderSystem::FindNode(const std::vector<Node*>& inNodes, const sf::Vector2i& coordinate)
	{
		for (auto* node : inNodes)
		{
			if (node->position == coordinate)
				return node;
		}
		return nullptr;
	}

	int SPathfinderSystem::GetHeuristicCost(const sf::Vector2i& inCoordinate, const sf::Vector2i& inTargetCoordinate)
	{
		sf::Vector2i delta(abs(inCoordinate.x - inTargetCoordinate.x), abs(inCoordinate.y - inTargetCoordinate.y));
		switch (heuristic)
		{
		case Heuristic::Euclidean:
			return 100 * (sqrt(pow(delta.x , 2) + pow(delta.y, 2)));
		case Heuristic::Manhattan:
			return 100 * (delta.x + delta.y);
		case Heuristic::ManhattanOctagonal:
			return 100 * (delta.x + delta.y) + (-60) * std::min(delta.x, delta.y);
		default:
			return 100;
		}
	}
}