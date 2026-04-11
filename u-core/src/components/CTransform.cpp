#include "CTransform.h"

#include <SFML/System/Vector2.hpp>
#include <imgui.h>
#include <UScene.h>

namespace uei 
{
    CTransform::CTransform() : UComponent(false) 
    {

    }

    CTransform::CTransform(const sf::Vector2i& inOriginalPosition) : CTransform()
    {
        originalPosition = inOriginalPosition;
    }

    CTransform::~CTransform()
    {

    }

    void CTransform::Start(UEngine& engine)
    {
        originalPosition = position;
        SetPosition(originalPosition);
    }

    void CTransform::SetPosition(const sf::Vector2i& inPosition)
    {
        positionLastUpdate = position;
        position = inPosition;
        gridPosition = PositionToGrid(position, uei::GridSize);
        pivotPosition = position + sf::Vector2i(uei::GridHalfSize, uei::GridHalfSize);
        pivotGridPosition = PositionToGrid(pivotPosition, uei::GridSize);
    }

    //bool CTransform::IsMoving(const float threshold) const
    //{
    //    sf::Vector2i delta = position - positionLastUpdate;
    //    return (delta.x * delta.x + delta.y * delta.y) > threshold * threshold;
    //}

    //bool CTransform::IsEqual(const sf::Vector2i& otherPosition, const float threshold) const
    //{
    //    sf::Vector2i delta = position - otherPosition;
    //    return (delta.x * delta.x + delta.y * delta.y) <= threshold * threshold;
    //}

    //float CTransform::Distance(const sf::Vector2i& otherPosition) const
    //{
    //    sf::Vector2i diff = otherPosition - position;
    //    return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    //}
}

