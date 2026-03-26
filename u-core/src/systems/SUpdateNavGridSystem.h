#pragma once
#include "USystem.h"

namespace uei
{
    class SUpdateNavGridSystem : public USystem
    {
    public:
        SUpdateNavGridSystem();

        virtual std::string SystemName() const { return "SUpdateNavGridSystem"; }
        virtual bool UpdateEditorScene() { return true; }
        virtual bool DrawEditorScene() { return true; }

        virtual void Update(UEngine& engine, std::vector<UEntity*> entities) override;
        virtual void Draw(UEngine& engine) override;
    };
}