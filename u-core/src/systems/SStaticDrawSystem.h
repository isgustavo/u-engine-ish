#pragma once
#include "USystem.h"

namespace uei
{
    class SStaticDrawSystem : public USystem
    {
    public:
        SStaticDrawSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;

    private:
        bool bIsDirty;
        sf::VertexArray vertexArray;
    };
}