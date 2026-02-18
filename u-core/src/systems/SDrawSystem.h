#pragma once
#include "USystem.h"

namespace uei
{
    class SDrawSystem : public USystem
    {
    public:
        SDrawSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;
    };
}