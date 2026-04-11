#pragma once
#include <systems/USystem.h>

namespace uei
{
    class SAnimationSystem : public USystem
    {
    public:
        SAnimationSystem();

        virtual std::string SystemName() const { return "SAnimationSystem"; }
        virtual bool UpdateEditorScene() { return true; }
        virtual bool DrawEditorScene() { return true; }

        virtual void Update(class UEngine& engine, std::vector<class UEntity*> entities) override;

    private:
        class SpriteAsset* CreateAnimationSpriteAsset(const class AnimationAsset* animationAsset, const float deltaTime);
    };
}