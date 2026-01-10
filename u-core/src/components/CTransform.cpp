#include "CTransform.h"
#include <SFML/System/Vector2.hpp>

namespace uei 
{
    //void CTransform::Update(const float deltaTime)
    //{
    //    SetPosition(position + (velocity * deltaTime));
    //}
    sf::Vector2f& CTransform::Position()
    {
        return position;
    }
    void CTransform::SetPosition(const sf::Vector2f& inPosition)
    {
        positionLastUpdate = position;
        position = inPosition;
        bUpdate = positionLastUpdate == inPosition;
    }
    /*void CTransform::SetVelocity(const sf::Vector2f& inVelocity)
    {
        velocity = inVelocity;
    }*/
    bool CTransform::ShouldUpdate() const
    {
        return bUpdate;
    }
}

