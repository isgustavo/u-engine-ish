#pragma once
#include "UEngine.h"

#include <memory>
#include <vector>


namespace uei 
{
    class USystem
    {
        friend class UEditorScene;
    public:
        USystem();
        ~USystem();

        virtual std::string SystemName() const = 0;
        virtual bool UpdateEditorScene() { return false; }
        virtual bool DrawEditorScene() { return false; }

        virtual void Update(UEngine& engine, std::vector<class UEntity*> entities) = 0;
        virtual void Draw(UEngine& engine) {}
    };
}