#pragma once
#include "USystem.h"
#include <SFML/System/Vector2.hpp>

namespace uei
{
    class SPathRequestSystem : public USystem
    {
    public:
        SPathRequestSystem();
        ~SPathRequestSystem();

        virtual std::string SystemName() const { return "SPathRequestSystem"; }

        virtual void Update(class UEngine& engine, std::vector<UEntity*> entities) override;

    };
}