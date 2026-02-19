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
		//CSprite(const sf::Texture& texture, const int x, const int y, const int width, const int height, const sf::Vector2f scale);
		CSprite(const sf::Texture& texture, uei::SpriteData inData, bool flipX);
		//CSprite(std::string inSpriteName);
		~CSprite();

		sf::Sprite* Sprite() { return sprite; }
		void SetScale(int gridSize);
		bool FlipX() { return bFlipX; }
		//void Sprite(sf::Sprite* value) { sprite = value; }
		//std::string SpriteName() { return spriteName; }

		UComponent* Clone() const override
		{
			CSprite* spriteCloned = new CSprite(sprite->getTexture(), data, bFlipX);
			return spriteCloned;
		}

		SpriteData& Data() { return data; }

		inline virtual std::string ComponentName() const override { return "CSprite"; }

		virtual void ShowEditor(UEngine& inEngine, bool bIsNew) override;
		virtual void LoadComponent(UEngine& engine, std::istream& in) override;
		virtual std::string Save() const override;

	private:
		//std::shared_ptr<sf::Sprite> sprite;
		//std::string spriteName;
		bool bFlipX;
		SpriteData data;
		sf::Sprite* sprite;
		//std::string selectedSpriteName;
	};
}