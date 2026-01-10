#pragma once

#include "entities/UEntity.h"

#include <vector>
#include <memory>

namespace uei 
{
    class USystem
    {
    public:
        USystem()
        {

        }

        virtual void Update(std::vector<std::unique_ptr<uei::UEntity>>& entities) = 0;
    };
}