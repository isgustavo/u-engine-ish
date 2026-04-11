#pragma once
#include "USystem.h"
#include "components/CMovementAnimation.h"

namespace uei
{
    class SGridMovementSystem : public USystem
    {
    public:
        SGridMovementSystem();

        sf::Vector2i LerpMovement(const sf::Vector2i& a, const sf::Vector2i& b, float t)
        {
            return sf::Vector2i(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
        }

        virtual std::string SystemName() const { return "SGridMovementSystem"; }
        virtual void Update(UEngine& engine, std::vector<UEntity*> entities) override;

    private:
        void SetDiagonalMovement(class CTransform* ctransform, class CGridMovement* cMovement, EMovement& movement);
    };
}