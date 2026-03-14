#pragma once
#include "USystem.h"

namespace uei
{
    class SDrawSystem : public USystem
    {
    public:
        SDrawSystem();
        ~SDrawSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;
    
    protected:
        std::unordered_map<std::string, class sf::VertexArray> vertexArrayMap;
        std::unordered_map <std::string, std::vector<class CTransform*>> transformMap;
        std::unordered_map <std::string, std::vector<class CSprite*>> spriteMap;
    };
}