#pragma once
#include "USystem.h"

namespace uei
{
    class SMovementSystem : public USystem
    {
    public:
        SMovementSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;

    private:
        void SetDiagonalMovement(class CTransform* transform, class CMovement* movement);
    };
}