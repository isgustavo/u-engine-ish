#pragma once
#include "UComponent.h"
#include "UAsset.h"
#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>

namespace uei
{
	class CSprite : public UComponent
	{

	public:
		CSprite();
		~CSprite();

		std::string GetSpriteName() const { return spriteName; }
		bool FlipX() const { return bFlipX; }
		bool FlipY() const { return bFlipY; }
		bool IsDirty() const { return bIsDirty; }
		void SetDirty(bool value) { bIsDirty = value; }

		SpriteAsset* GetSpriteAsset() { return spriteAsset; }
		void SetSpriteAsset(SpriteAsset* inSpriteAsset);

		UComponent* Clone() override
		{
			return new CSprite(spriteName, bFlipX, bFlipY);
		}

		virtual void Start(class UEngine& engine) override;

		inline std::string ComponentName() const override { return "CSprite"; }

		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize() const override;

	private:
		CSprite(std::string& inSpriteName, bool inFlipX, bool inFlipY);

		SpriteAsset* spriteAsset;
		
		std::string spriteName;
		bool bFlipX, bFlipY;
		bool bIsDirty;
	};
}