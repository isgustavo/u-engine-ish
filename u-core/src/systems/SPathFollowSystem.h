#pragma once
#include "USystem.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
    class SPathFollowSystem : public USystem
    {
    public:
        SPathFollowSystem();
        ~SPathFollowSystem();

        virtual std::string SystemName() const { return "SPathFollowSystem"; }

        virtual void Update(class UEngine& engine, std::vector<UEntity*> entities) override;
        virtual void Draw(UEngine& engine) override;

    private:
        bool bDraw;
        std::vector<std::vector<sf::Vector2i>> paths;
    };
}