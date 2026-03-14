#pragma once
#include "USystem.h"

namespace uei
{
    class SAnimationSystem : public USystem
    {
    public:
        SAnimationSystem();

        virtual void Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities) override;

    private:
        SpriteAsset* CreateAnimationSpriteAsset(const AnimationAsset* animationAsset, const float deltaTime);
    };
}