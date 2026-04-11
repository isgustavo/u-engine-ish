#pragma once
#include <string>
#include <memory>
#include <vector>

namespace uei 
{
    class USystem
    {
    public:
        USystem();
        ~USystem();

        virtual std::string SystemName() const = 0;

        virtual bool UpdateEditorScene() { return false; }
        virtual bool DrawEditorScene() { return false; }

        virtual void Update(class UEngine& engine, std::vector<class UEntity*> entities) = 0;
        virtual void Draw(class UEngine& engine) {}
    };
}