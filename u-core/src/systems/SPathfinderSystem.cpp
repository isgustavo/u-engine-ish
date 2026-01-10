#include "SPathfinderSystem.h"

#include <components/CAgent.h>
#include <components/CPathRequest.h>
#include <components/CPath.h>
#include <entities/UEntity.h>
#include <vector>
#include <unordered_map>
#include <components/CTransform.h>
#include <iostream>

namespace uei
{
	Node::Node(const sf::Vector2i& inPosition, Node* inParent)
	{
		parent = inParent;
		position = inPosition;
		G = H = 0;
	}
	int Node::GetScore()
	{
		return G + H;
	}
	SPathfinderSystem::SPathfinderSystem(Heuristic inHeuristic, std::vector<int>& inNavGrid, 
											int inNavGridColumnSize, int inNavGridSqrSize) : USystem(),
		heuristic(inHeuristic), navGrid(inNavGrid), navGridColumnSize(inNavGridColumnSize), navGridSqrSize(inNavGridSqrSize)
	{
		openVector.reserve(navGrid.size());
		closeVector.reserve(navGrid.size());
		SetGridCoordinateMoves();
	}
	void SPathfinderSystem::SetGridCoordinateMoves()
	{
		gridCoordinateMoves.push_back(sf::Vector2i(0, 1));
		gridCoordinateMoves.push_back(sf::Vector2i(1, 0));
		gridCoordinateMoves.push_back(sf::Vector2i(0, -1));
		gridCoordinateMoves.push_back(sf::Vector2i(-1, 0));
		if (heuristic != Heuristic::Manhattan)
		{
			gridCoordinateMoves.push_back(sf::Vector2i(-1, -1));
			gridCoordinateMoves.push_back(sf::Vector2i(1, 1));
			gridCoordinateMoves.push_back(sf::Vector2i(-1, 1));
			gridCoordinateMoves.push_back(sf::Vector2i(1, -1));
		}
	}
	void SPathfinderSystem::Update(std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		for (auto& e : entities)
		{
			auto c_agent = e.get()->GetComponent<uei::CAgent>();
			auto c_transform = e.get()->GetComponent<uei::CTransform>();
			auto c_pathRequest = e.get()->GetComponent<uei::CPathRequest>();

			if (c_agent == nullptr || c_pathRequest == nullptr || c_transform == nullptr) continue;

			for (int i = 0; i < navGrid.size(); i++)
			{
				std::cout << navGrid[i] << ",";
			}

			e->AddComponent<uei::CPath>(
				FindPath(sf::Vector2i(c_transform->Position().x / navGridSqrSize, c_transform->Position().y / navGridSqrSize),
					sf::Vector2i(c_pathRequest->TargetPosition().x / navGridSqrSize, c_pathRequest->TargetPosition().y / navGridSqrSize),
						c_agent->GridSize())
			);
			e->RemoveComponent<uei::CPathRequest>();
		}
	}

	const std::vector<sf::Vector2i> SPathfinderSystem::FindPath(const sf::Vector2i& inSourceCoordinate, const sf::Vector2i& inTargetCoordinate, const sf::Vector2i& inSourceSize)
	{
		std::cout << "FindPath" << std::endl;
		std::cout << inSourceCoordinate.x << "," << inSourceCoordinate.y << std::endl;
		std::cout << inTargetCoordinate.x << "," << inTargetCoordinate.y << std::endl;
		std::cout << "Start" << std::endl;
		uei::Node* currentNode = nullptr;
		openVector.push_back(new Node(inSourceCoordinate, nullptr));

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

			if (currentNode->position == inTargetCoordinate)
			{
				break;
			}

			closeVector.push_back(currentNode);
			openVector.erase(current_it);

			for (size_t i = 0; i < gridCoordinateMoves.size(); i++)
			{
				const sf::Vector2i newCoordinate(currentNode->position + gridCoordinateMoves[i]);
				//std::cout << newCoordinate.x << "," << newCoordinate.y << std::endl;
				//const int newCoordinateIndex = (newCoordinate.x * navGridColumnSize) + newCoordinate.y;
				if (IsOutOfNavGrid(currentNode->position, gridCoordinateMoves[i], inSourceSize) ||
					FindNode(closeVector, newCoordinate))
				{
					continue;
				}

				const int newCoordinateIndex = (newCoordinate.x * navGridColumnSize) + newCoordinate.y;
				const int newCoordinateCost = currentNode->G + (((i < 4) ? 100 : 142) + navGrid[newCoordinateIndex]);

				Node* newCoordinateNode = FindNode(openVector, newCoordinate);
				if (newCoordinateNode == nullptr)
				{
					newCoordinateNode = new Node(newCoordinate, currentNode);
					newCoordinateNode->G = newCoordinateCost;
					newCoordinateNode->H = GetHeuristicCost(newCoordinate, inTargetCoordinate);
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
			std::cout << currentNode->position.x << "," << currentNode->position.y << std::endl;
			path.push_back(currentNode->position);
			currentNode = currentNode->parent;
		}

		for (auto it = openVector.begin(); it != openVector.end();) 
		{
			delete* it;
			it = openVector.erase(it);
		}

		for (auto it = closeVector.begin(); it != closeVector.end();)
		{
			delete* it;
			it = closeVector.erase(it);
		}

		return path;
	}
	bool SPathfinderSystem::IsOutOfNavGrid(const sf::Vector2i& inCoordinate, const sf::Vector2i& inMove, const sf::Vector2i& inSize)
	{
		const sf::Vector2i newCoordinate(inCoordinate + inMove);
		bool isDiagonalMove = inMove.x != 0 && inMove.y != 0;
		if (isDiagonalMove)
		{
			if ((navGrid[(inCoordinate.x + inMove.x) * navGridColumnSize + inCoordinate.y] == -1) ||
				(navGrid[(inCoordinate.x * navGridColumnSize) + inCoordinate.y + inMove.y] == -1))
			{
				std::cout << "IsOutOfNavGrid at Diagonal == -1" << std::endl;
				return true;
			}
		}
		for (int i = newCoordinate.x; i < newCoordinate.x + inSize.x; i++)
		{
			for (int j = newCoordinate.y; j < newCoordinate.y + inSize.y; j++)
			{
				if (i < 0 || i >= navGridColumnSize ||
					j < 0 || j >= navGridColumnSize)
				{
					std::cout << "IsOutOfNavGrid" << std::endl;
					return true;
				}

				if (navGrid[i * navGridColumnSize + j] == -1)
				{
					std::cout << "IsOutOfNavGrid == -1" << std::endl;
					return true;
				}
			}
		}

		return false;
	}
	uei::Node* SPathfinderSystem::FindNode(const std::vector<uei::Node*>& inNodes, const sf::Vector2i& coordinate)
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
		}
	}
}