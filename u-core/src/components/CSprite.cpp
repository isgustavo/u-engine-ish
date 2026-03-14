#include "components/CSprite.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <imgui.h>

namespace uei
{
	CSprite::CSprite() : UComponent(true), bFlipX(false), bFlipY(false)
	{
		spriteName = EMPTY;
	}

	CSprite::CSprite(std::string& inSpriteName, bool inFlipX, bool inFlipY) : CSprite()
	{
		spriteName = inSpriteName;
		bFlipX = inFlipX;
		bFlipY = inFlipY;
		//SetSpriteAsset(inSpriteAsset);
	}

	CSprite::~CSprite()
	{
		delete spriteAsset;
		spriteAsset = nullptr;
	}

	void CSprite::Start(UEngine& engine)
	{
		if (spriteName != EMPTY)
			SetSpriteAsset(new SpriteAsset(&engine.Assets()->GetSpriteAsset(spriteName)));
	}

	void CSprite::SetSpriteAsset(SpriteAsset* inSpriteAsset)
	{
		delete spriteAsset;
		spriteAsset = nullptr;

		//if (inSpriteAsset != nullptr)
		{
			spriteAsset = inSpriteAsset;
			SetDirty(true);
		}
	}

	void CSprite::OnShowEditor(UEngine& engine)
	{
		if (IsRequiredByOtherComponent())
		{
			return;
		}

		auto* assets = engine.Assets();
		const auto& allSprites = assets->SpriteNames();

		int selectedIndex = -1;

		//for (int i = 0; i < allSprites.size(); i++)
		//{
		//	if (spriteAsset != nullptr)
		//	{
		//		if (allSprites[i] == spriteAsset->AssetName)
		//		{
		//			selectedIndex = i;
		//			break;
		//		}
		//	}
		//}

		//if (ImGui::BeginCombo("##Sprite", (selectedIndex == -1) ? " " : allSprites[selectedIndex].c_str()))
		if (ImGui::BeginCombo("##Sprite", spriteName.c_str()))
		{
			for (int i = 0; i < allSprites.size(); ++i) {
				const bool isSelected = (selectedIndex == i);
				if (ImGui::Selectable(allSprites[i].c_str(), isSelected))
				{
					selectedIndex = i;
					//if (sprite != nullptr)
					//{
					//	delete sprite;
					//	sprite = nullptr;
					//}
					auto& spriteAsset = assets->GetSpriteAsset(allSprites[i]);
					spriteName = spriteAsset.AssetName;
					//sprite = new sf::Sprite(assets->GetTexture(spriteAsset.textureName), sf::IntRect({ spriteAsset.x, spriteAsset.y }, { spriteAsset.width, spriteAsset.height }));
					//spriteName = allSprites[i];
				}
			}
			ImGui::EndCombo();
		}

		//if (sprite != nullptr)
		//if(spriteAsset != nullptr)
		if(spriteName != EMPTY)
		{
			ImGui::Checkbox("FlipX", &bFlipX);
			ImGui::SameLine();
			ImGui::Checkbox("FlipY", &bFlipY);

			//uei::SpriteAsset spriteAsset = assets->GetSpriteAsset(spriteAsset->SpriteName);
			auto& spriteAsset = assets->GetSpriteAsset(spriteName);
			sf::Sprite sprite = sf::Sprite(assets->GetTexture(spriteAsset.TextureName),
				sf::IntRect({ spriteAsset.X, spriteAsset.Y }, { spriteAsset.Width, spriteAsset.Height }));

			ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
			sf::Vector2u size = sprite.getTexture().getSize();

			ImGui::Spacing();
			ImGui::Text(spriteName.c_str());
			ImGui::Spacing();

			ImVec2 uv0(
				bFlipX ? (float)(spriteAsset.X + spriteAsset.Width) / size.x : (float)spriteAsset.X / size.x,
				bFlipY ? (float)(spriteAsset.Y + spriteAsset.Height) / size.y : (float)spriteAsset.Y / size.y
			);

			ImVec2 uv1(
				bFlipX ? (float)spriteAsset.X / size.x : (float)(spriteAsset.X + spriteAsset.Width) / size.x,
				bFlipY ? (float)spriteAsset.Y / size.y : (float)(spriteAsset.Y + spriteAsset.Height) / size.y
			);

			ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
		}
	}

	int uei::CSprite::GetEditorSize() const
	{
		return IsRequiredByOtherComponent() ? 33 : 200;
	}

	void uei::CSprite::LoadComponent(std::istream& in)
	{
		std::string flipX;
		std::string flipY;
		//std::string spriteName;
		//in >> spriteName >> flipX >> flipY;
		//SetSpriteAsset(spriteName != EMPTY ? new SpriteAsset(&engine.Assets()->GetSpriteAsset(spriteName)) : nullptr);

		Deserialize(in, spriteName, flipX, flipY);
		bFlipX = flipX == "1" ? true : false;
		bFlipY = flipY == "1" ? true : false;
	}

	std::string CSprite::SaveComponent() const
	{
		return Serialize(spriteName, bFlipX ? "1" : "0", bFlipY ? "1" : "0");
		//return (spriteAsset == nullptr ? EMPTY : spriteAsset->AssetName) + " " + (bFlipX ? "1" : "0") + " " + (bFlipY ? "1" : "0");
	}
}