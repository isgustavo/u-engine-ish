#pragma once
#include "UComponent.h"

#include <SFML/Graphics/Sprite.hpp>

#include <memory>
#include <string>
#include <UAsset.h>

namespace uei
{
	class CSprite : public UComponent
	{
	public:
		CSprite();
		~CSprite();

		sf::Sprite* Sprite() { return sprite; }
		void SetScale(int gridSize);
		bool FlipX() { return bFlipX; }
		bool FlipY() { return bFlipY; }
		std::string SpriteName() { return spriteName; }

		UComponent* Clone() const override
		{
			CSprite* spriteCloned = new CSprite(spriteName, bFlipX, bFlipY);
			spriteCloned->sprite = new sf::Sprite(*sprite);
			return spriteCloned;
		}

		inline virtual std::string ComponentName() const override { return "CSprite"; }

		virtual void ShowEditor(UEngine& inEngine, bool bIsNew) override;

		virtual void LoadComponent(UEngine& engine, std::istream& in) override;
		virtual std::string Save() const override;

	private:
		CSprite(std::string inSpriteName, bool inFlipX, bool inFlipY);

		std::string spriteName;
		bool bFlipX, bFlipY;
		sf::Sprite* sprite;
	};
}