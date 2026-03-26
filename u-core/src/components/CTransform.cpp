#include "CTransform.h"

#include <SFML/System/Vector2.hpp>
#include <imgui.h>

namespace uei 
{
    CTransform::CTransform() : UComponent(false) 
    {

    }

    CTransform::CTransform(const sf::Vector2f& inOriginalPosition, const sf::Vector2f& inPosition) : CTransform()
    {
        originalPosition = inOriginalPosition;
        positionLastUpdate = inPosition;
        position = inPosition;
    }

    CTransform::~CTransform()
    {

    }

    void CTransform::Start(UEngine& engine)
    {
        originalPosition = position;
    }

    sf::Vector2f& CTransform::GetPosition()
    {
        return position;
    }

    sf::Vector2f& CTransform::GetPositionLastUpdate()
    {
        return positionLastUpdate;
    }

    void CTransform::SetPosition(const sf::Vector2f& inPosition)
    {
        positionLastUpdate = position;
        position = inPosition;
    }

    bool CTransform::IsMoving(const float threshold) const
    {
        sf::Vector2f delta = position - positionLastUpdate;
        return (delta.x * delta.x + delta.y * delta.y) > threshold * threshold;
    }

    bool CTransform::IsEqual(const sf::Vector2f& otherPosition, const float threshold) const
    {
        sf::Vector2f delta = position - otherPosition;
        return (delta.x * delta.x + delta.y * delta.y) <= threshold * threshold;
    }
}

