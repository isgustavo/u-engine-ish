#include "CTransform.h"

#include <SFML/System/Vector2.hpp>
#include <imgui.h>
#include <fstream>
#include <sstream>

namespace uei 
{
    CTransform::CTransform() : UComponent(false) 
    {

    }

    CTransform::CTransform(const sf::Vector2f& inPosition) : CTransform()
    {
        positionLastUpdate = inPosition;
        position = inPosition;
    }

    CTransform::~CTransform()
    {

    }

    sf::Vector2f& uei::CTransform::GetPosition()
    {
        return position;
    }

    sf::Vector2f& uei::CTransform::GetPositionLastUpdate()
    {
        return positionLastUpdate;
    }

    void CTransform::SetPosition(const sf::Vector2f& inPosition)
    {
        positionLastUpdate = position;
        position = inPosition;
    }

    bool CTransform::IsMoving(const float threshould) const
    {
        sf::Vector2f delta = position - positionLastUpdate;
        return (delta.x * delta.x + delta.y * delta.y) > threshould * threshould;
    }

    void CTransform::OnShowEditor(UEngine& engine)
    {

    }
    
    void uei::CTransform::LoadComponent(std::istream & in)
    {
        //float x, y;
        //in >> x >> y;
        //position = sf::Vector2f(x, y);
        Deserialize(in, position.x, position.y);
        positionLastUpdate = position;
    }

    std::string CTransform::SaveComponent() const
    {
        //std::ostringstream ss;
        //ss << position.x << " " << position.y;
        //return ss.str();
        return Serialize(position.x, position.y);
    }
}

