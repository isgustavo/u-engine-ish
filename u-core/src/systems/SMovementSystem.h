#pragma once
#include "USystem.h"
#include "components/CMovementAnimation.h"

namespace uei
{
    class SMovementSystem : public USystem
    {
    public:
        SMovementSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;

    private:
        void SetDiagonalMovement(class CTransform* ctransform, class CMovement* cMovement, EMovement& movement);
    };
}