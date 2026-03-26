#pragma once
#include "USystem.h"
#include <SFML/System/Vector2.hpp>
#include <components/CMovementAnimation.h>

namespace uei
{
    enum Heuristic
    {
        None,
        Manhattan,
        ManhattanOctagonal,
        Euclidean
    };

    struct Node
    {
        int G, H;
        sf::Vector2i position;
        Node* parent;

        Node(const sf::Vector2i& inPosition, Node* inParent = nullptr);
        int GetScore() const;
    };

    class SPathfinderSystem : public USystem
    {
    public:
        SPathfinderSystem();

        virtual std::string SystemName() const { return "SPathfinderSystem"; }
        virtual bool UpdateEditorScene() { return true; }
        virtual bool DrawEditorScene() { return true; }

        virtual void Update(UEngine& engine, std::vector<UEntity*> entities) override;

    private:
        Heuristic heuristic;
        std::vector<Node*> openVector;
        std::vector<Node*> closeVector;

        std::vector<sf::Vector2i> FindPath(const std::vector<int>& inNavGrid, 
            const sf::Vector2i& targetGridPosition, const sf::Vector2i& agentGridPosition,
            const std::vector<sf::Vector2i>& agentInitialInvalidGridMovement,
            const std::vector<sf::Vector2i>& agentValidGridMovement,
            const int agentGridColumn, const int agentGridRow, const int gridColumns);

        bool IsOutOfNavGrid(const std::vector<int>& inNavGrid, const int targetIndex, 
            const sf::Vector2i& currentNode, 
            const sf::Vector2i& nextMovement,
            const int agentGridColumn, const int agentGridRow, const int gridColumns);

        Node* FindNode(const std::vector<Node*>& inNodes, const sf::Vector2i& coordinate);

        int GetHeuristicCost(const sf::Vector2i& inCoordinate, const sf::Vector2i& inTargetCoordinate);
    };
}
