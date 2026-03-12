#pragma once
#include "UComponent.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>
#include "UAsset.h"

namespace uei
{
	class CSprite : public UComponent
	{
	public:
		CSprite();
		~CSprite();

		//sf::Sprite* Sprite() { return sprite; }
		//std::string SpriteName() { return spriteName; }
		void SetSpriteAsset(SpriteAsset* inSpriteAsset);
		SpriteAsset* GetSpriteAsset() { return spriteAsset; }
		bool FlipX() const { return bFlipX; }
		bool FlipY() const { return bFlipY; }
		bool IsDirty() const { return bIsDirty; }
		void SetDirty(bool value) { bIsDirty = value; }

		UComponent* Clone() const override
		{
			return new CSprite(spriteAsset, bFlipX, bFlipY);
		}

		inline std::string ComponentName() const override { return "CSprite"; }

		void LoadComponent(class UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		CSprite(SpriteAsset* inSpriteAsset, bool inFlipX, bool inFlipY);

		SpriteAsset* spriteAsset;
		bool bFlipX, bFlipY;
		bool bIsDirty;
		//sf::Sprite* sprite;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}