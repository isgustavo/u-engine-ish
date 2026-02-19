#include "components/CSprite.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <imgui.h>

namespace uei
{
	CSprite::CSprite() : UComponent()
	{
		sprite = nullptr;
	}
	CSprite::CSprite(std::string inSpriteName, bool inFlipX, bool inFlipY) : UComponent()
	{
		spriteName = inSpriteName;
		bFlipX = inFlipX;
		bFlipY = inFlipY;
		sprite = nullptr;
	}
	CSprite::~CSprite()
	{
		delete sprite;
		sprite = nullptr;
	}

	void CSprite::SetScale(int gridSize)
	{
		sprite->setScale(sf::Vector2f((bFlipX ? -1 : 1) * (gridSize / sprite->getTextureRect().size.x), 
			(bFlipY ? -1 : 1) * gridSize / sprite->getTextureRect().size.y));
	}

	void CSprite::ShowEditor(UEngine& engine, bool bIsNew)
	{
		if (bIsNew)
		{
			ImGui::BeginChild(
				"Sprite",
				ImVec2(0, 180 + engine.CurrentScene()->GridSize()),
				true
			);

			ImGui::Text("Sprite");
			ImGui::SameLine();

			auto* assets = engine.Assets();
			const auto& allSprites = assets->SpritesNames();

			int selectedIndex = -1;

			for (int i = 0; i < allSprites.size(); i++)
			{
				if (allSprites[i] == spriteName)
				{
					selectedIndex = i;
					break;
				}
			}

			if (ImGui::BeginCombo("##Sprite", (selectedIndex == -1) ? " " : allSprites[selectedIndex].c_str()))
			{
				for (int i = 0; i < allSprites.size(); ++i) {
					const bool isSelected = (selectedIndex == i);
					if (ImGui::Selectable(allSprites[i].c_str(), isSelected))
					{
						selectedIndex = i;
						if (sprite != nullptr)
						{
							delete sprite;
							sprite = nullptr;
						}
						uei::SpriteAsset spriteAsset = assets->GetSpriteAsset(allSprites[i]);
						spriteName = spriteAsset.name;
						sprite = new sf::Sprite(assets->GetTexture(spriteAsset.textureName), sf::IntRect({ spriteAsset.x, spriteAsset.y }, { spriteAsset.width, spriteAsset.height }));
					}
				}
				ImGui::EndCombo();
			}

			if (sprite != nullptr)
			{
				ImGui::Checkbox("FlipX", &bFlipX);
				ImGui::SameLine();
				ImGui::Checkbox("FlipY", &bFlipY);

				ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
				sf::Vector2u size = sprite->getTexture().getSize();

				ImGui::Spacing();
				ImGui::Text(spriteName.c_str());
				ImGui::Spacing();

				uei::SpriteAsset spriteAsset = assets->GetSpriteAsset(spriteName);
				ImVec2 uv0(
					bFlipX ? (float)(spriteAsset.x + spriteAsset.width) / size.x : (float)spriteAsset.x / size.x,
					bFlipY ? (float)(spriteAsset.y + spriteAsset.height) / size.y : (float)spriteAsset.y / size.y
				);

				ImVec2 uv1(
					bFlipX ? (float)spriteAsset.x / size.x : (float)(spriteAsset.x + spriteAsset.width) / size.x,
					bFlipY ? (float)spriteAsset.y / size.y : (float)(spriteAsset.y + spriteAsset.height) / size.y
				);

				ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
			}

			ImGui::EndChild();
			ImGui::Spacing();
		}
	}

	void CSprite::LoadComponent(UEngine& engine, std::istream& in)
	{
		std::string flipX;
		std::string flipY;
		in >> spriteName >> flipX >> flipY;
		bFlipX = flipX == "1" ? true : false;
		bFlipY = flipY == "1" ? true : false;
		uei::SpriteAsset data = engine.Assets()->GetSpriteAsset(spriteName);
		sprite = new sf::Sprite(engine.Assets()->GetTexture(data.textureName), sf::IntRect({ data.x, data.y }, { data.width, data.height }));
		sprite->setScale(sf::Vector2f((bFlipX ? -1 : 1) * sprite->getTextureRect().size.x, (bFlipY ? -1 : 1) * sprite->getTextureRect().size.y));
	}

	std::string CSprite::Save() const
	{
		return spriteName + " " + (bFlipX ? "1" : "0") + " " + (bFlipY ? "1" : "0");
	}
}

