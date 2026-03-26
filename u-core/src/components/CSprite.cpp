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
		spriteAsset = nullptr;
		bIsDirty = false;
	}

	CSprite::CSprite(std::string& inSpriteName, bool inFlipX, bool inFlipY) : CSprite()
	{
		spriteName = inSpriteName;
		bFlipX = inFlipX;
		bFlipY = inFlipY;
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

		spriteAsset = inSpriteAsset;
		SetDirty(true);
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

		if (ImGui::BeginCombo("##Sprite", spriteName.c_str()))
		{
			for (int i = 0; i < allSprites.size(); ++i) {
				const bool isSelected = (selectedIndex == i);
				if (ImGui::Selectable(allSprites[i].c_str(), isSelected))
				{
					selectedIndex = i;
					auto& spriteAsset = assets->GetSpriteAsset(allSprites[i]);
					spriteName = spriteAsset.AssetName;
				}
			}
			ImGui::EndCombo();
		}

		if(spriteName != EMPTY)
		{
			ImGui::Checkbox("FlipX", &bFlipX);
			ImGui::SameLine();
			ImGui::Checkbox("FlipY", &bFlipY);

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

		Deserialize(in, spriteName, flipX, flipY);
		bFlipX = flipX == "1" ? true : false;
		bFlipY = flipY == "1" ? true : false;
	}

	std::string CSprite::SaveComponent() const
	{
		return Serialize(spriteName, bFlipX ? "1" : "0", bFlipY ? "1" : "0");
	}
}