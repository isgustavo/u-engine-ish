#pragma once
#include "UComponent.h"
#include "UEngine.h"

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
		std::string SpriteName() { return spriteName; }
		bool FlipX() { return bFlipX; }
		bool FlipY() { return bFlipY; }

		void SetScale(int gridSize);

		UComponent* Clone() const override
		{
			CSprite* s = nullptr;
			if (spriteName != EMPTY)
			{
				s = new CSprite(spriteName, bFlipX, bFlipY);
				s->sprite = new sf::Sprite(*sprite);
			}

			return s;
		}

		inline std::string ComponentName() const override { return "CSprite"; }

		void LoadComponent(class UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		CSprite(std::string inSpriteName, bool inFlipX, bool inFlipY);

		std::string spriteName;
		bool bFlipX, bFlipY;
		sf::Sprite* sprite;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}