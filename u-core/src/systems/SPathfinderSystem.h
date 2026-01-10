#pragma once
#include "USystem.h"
#include <SFML/System/Vector2.hpp>

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
        int GetScore();
    };

    class SPathfinderSystem : public USystem
    {
    public:
        SPathfinderSystem(Heuristic inHeuristic, std::vector<int>& inNavGrid, int inNavGridColumnSize, int inNavGridSqrSize);

        virtual void Update(std::vector<std::unique_ptr<uei::UEntity>>& entities) override;

    private:
        Heuristic heuristic;
        std::vector<sf::Vector2i> gridCoordinateMoves;
        std::vector<int>& navGrid;
        int navGridColumnSize;
        int navGridSqrSize;
        std::vector<uei::Node*> openVector;
        std::vector<uei::Node*> closeVector;

        const std::vector<sf::Vector2i> FindPath(const sf::Vector2i& inSourceCoordinate, const sf::Vector2i& inTargetCoordinate);
        void SetGridCoordinateMoves();
        bool IsOutOfNavGrid(const sf::Vector2i& inCoordinate);
        uei::Node* FindNode(const std::vector<uei::Node*>& inNodes, const sf::Vector2i& coordinate);
        int GetHeuristicCost(const sf::Vector2i& inCoordinate, const sf::Vector2i& inTargetCoordinate);
    };
}
