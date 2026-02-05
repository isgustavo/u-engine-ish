#include "CTransform.h"

#include <SFML/System/Vector2.hpp>
#include <imgui.h>
#include <fstream>
#include <sstream>

namespace uei 
{
    CTransform::CTransform() : UComponent() {}

    CTransform::CTransform(const sf::Vector2f& inPosition/*, sf::Vector2f& inInitialVelocity*/) : UComponent(),
        position(inPosition), positionLastUpdate(position), bUpdate(true)/*, velocity(inInitialVelocity)*/
    {

    }

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
    void CTransform::Editor_Show(UEngine& inEngine, bool bIsNew)
    {
        ImGui::BeginChild(
            "Transform",
            ImVec2(0, bIsNew ? 33 : 100),
            true
        );
        ImGui::Text("CTransform");
        if (!bIsNew)
        {
            float pos[2] = { position.x, position.y };
            ImGui::Text("Position");
            ImGui::SameLine();
            if (ImGui::InputFloat2("##t001", pos))
            {
                position = { pos[0], pos[1] };
            }
        }
        ImGui::EndChild();
        ImGui::Spacing();
    }
    void CTransform::Editor_Load(std::istream& in)
    {
        float x, y;
        in >> x >> y;
        SetPosition(sf::Vector2f(x, y));
    }
    std::string CTransform::Editor_Save() const
    {
        std::ostringstream ss;
        ss << position.x << " " << position.y;
        return ss.str();
    }
}

