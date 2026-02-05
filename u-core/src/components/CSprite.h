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
		CSprite(const sf::Texture& texture, const int x, const int y, const int width, const int height, const sf::Vector2f scale);
		CSprite(std::string inSpriteName);

		~CSprite();

		sf::Sprite* Sprite() { return sprite; }
		void Sprite(sf::Sprite* value) { sprite = value; }
		std::string SpriteName() { return spriteName; }

		UComponent* Clone() const override
		{
			return new CSprite(this->spriteName);
		}

		inline virtual std::string Editor_ComponentName() const override { return "CSprite"; }
		virtual void Editor_Show(UEngine& inEngine, bool bIsNew) override;
		virtual void Editor_Load(std::istream& in) override;
		virtual std::string Editor_Save() const override;

	private:
		//std::shared_ptr<sf::Sprite> sprite;
		std::string spriteName;
		sf::Sprite* sprite;

		int editor_selectedIndex = 0;
		uei::SpriteData editor_data;
		//std::string editor_selectedSpriteName;
	};
}