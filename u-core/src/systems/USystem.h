#pragma once

#include "entities/UEntity.h"

#include <memory>
#include <vector>
#include <UEngine.h>

namespace uei 
{
    class USystem
    {
    public:
        USystem();
        ~USystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) = 0;
    };
}