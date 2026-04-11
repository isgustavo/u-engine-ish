#pragma once
#include "USystem.h"
#include <unordered_map>
#include <SFML/Graphics/VertexArray.hpp>

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

        virtual void Update(class UEngine& engine, std::vector<class UEntity*> entities) override;
        virtual void Draw(class UEngine& engine) override;

    protected:
        std::unordered_map<std::string, sf::VertexArray> vertexArrayMap;
        std::unordered_map <std::string, std::vector<class CTransform*>> transformMap;
        std::unordered_map <std::string, std::vector<class CSprite*>> spriteMap;
    };
}