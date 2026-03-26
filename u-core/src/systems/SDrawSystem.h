#pragma once
#include "USystem.h"

namespace uei
{
    class SDrawSystem : public USystem
    {
    public:
        SDrawSystem();
        ~SDrawSystem();

        virtual std::string SystemName() const { return "SDrawSystem"; }
        virtual bool UpdateEditorScene() { return true; }
        virtual bool DrawEditorScene() { return true; }

        virtual void Update(UEngine& engine, std::vector<UEntity*> entities) override;
        virtual void Draw(UEngine& engine) override;

    protected:
        std::unordered_map<std::string, class sf::VertexArray> vertexArrayMap;
        std::unordered_map <std::string, std::vector<class CTransform*>> transformMap;
        std::unordered_map <std::string, std::vector<class CSprite*>> spriteMap;
    };
}