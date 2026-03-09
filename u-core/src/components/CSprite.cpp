#include "components/CSprite.h"
#include "UEngine.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <iostream>
#include <imgui.h>

namespace uei
{
	CSprite::CSprite() : UComponent(), spriteAsset(nullptr), bFlipX(false), bFlipY(false)
	{
		//sprite = nullptr;
		//bFlipX = false;
		//bFlipY = false;
	}

	//CSprite::CSprite(std::string inSpriteName, bool inFlipX, bool inFlipY) : UComponent()
	CSprite::CSprite(const SpriteAsset* inSpriteAsset, bool inFlipX, bool inFlipY) : UComponent(),
		spriteAsset(new SpriteAsset(inSpriteAsset)), bFlipX(inFlipX), bFlipY(inFlipY)
	{
		//spriteName = inSpriteName;
		//bFlipX = inFlipX;
		//bFlipY = inFlipY;
		//sprite = nullptr;
	}

	CSprite::CSprite(bool inFlipX, bool inFlipY) : UComponent(), bFlipX(false), bFlipY(false)
	{
		//sprite = nullptr;
		//bFlipX = false;
		//bFlipY = false;
	}

	CSprite::~CSprite()
	{
		delete spriteAsset;
		spriteAsset = nullptr;
	}

	void CSprite::SetSpriteAsset(SpriteAsset* inSpriteAsset)
	{
		delete spriteAsset;
		spriteAsset = nullptr;

		spriteAsset = inSpriteAsset;
	}

	//void CSprite::SetScale(int gridSize)
	//{
	//	sprite->setScale(sf::Vector2f((bFlipX ? -1 : 1) * (gridSize / sprite->getTextureRect().size.x), 
	//		(bFlipY ? -1 : 1) * gridSize / sprite->getTextureRect().size.y));
	//}

	void CSprite::OnShowEditor(UEngine& engine)
	{
		if (bIsRequiredByOtherComponent)
		{
			return;
		}

		auto* assets = engine.Assets();
		const auto& allSprites = assets->SpriteNames();

		int selectedIndex = -1;

		for (int i = 0; i < allSprites.size(); i++)
		{
			if (spriteAsset != nullptr)
			{
				if (allSprites[i] == spriteAsset->AssetName)
				{
					selectedIndex = i;
					break;
				}
			}
		}

		if (ImGui::BeginCombo("##Sprite", (selectedIndex == -1) ? " " : allSprites[selectedIndex].c_str()))
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
					spriteAsset = new SpriteAsset(&assets->GetSpriteAsset(allSprites[i]));
					//spriteName = spriteAsset.name;
					//sprite = new sf::Sprite(assets->GetTexture(spriteAsset.textureName), sf::IntRect({ spriteAsset.x, spriteAsset.y }, { spriteAsset.width, spriteAsset.height }));
					//spriteName = allSprites[i];
				}
			}
			ImGui::EndCombo();
		}

		//if (sprite != nullptr)
		if(spriteAsset != nullptr)
		{
			ImGui::Checkbox("FlipX", &bFlipX);
			ImGui::SameLine();
			ImGui::Checkbox("FlipY", &bFlipY);

			//uei::SpriteAsset spriteAsset = assets->GetSpriteAsset(spriteAsset->SpriteName);
			sf::Sprite sprite = sf::Sprite(assets->GetTexture(spriteAsset->TextureName),
				sf::IntRect({ spriteAsset->X, spriteAsset->Y }, { spriteAsset->Width, spriteAsset->Height }));

			ImTextureID id = (ImTextureID)(intptr_t)sprite.getTexture().getNativeHandle();
			sf::Vector2u size = sprite.getTexture().getSize();

			ImGui::Spacing();
			ImGui::Text(spriteAsset->AssetName.c_str());
			ImGui::Spacing();

			ImVec2 uv0(
				bFlipX ? (float)(spriteAsset->X + spriteAsset->Width) / size.x : (float)spriteAsset->X / size.x,
				bFlipY ? (float)(spriteAsset->Y + spriteAsset->Height) / size.y : (float)spriteAsset->Y / size.y
			);

			ImVec2 uv1(
				bFlipX ? (float)spriteAsset->X / size.x : (float)(spriteAsset->X + spriteAsset->Width) / size.x,
				bFlipY ? (float)spriteAsset->Y / size.y : (float)(spriteAsset->Y + spriteAsset->Height) / size.y
			);

			ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
		}
	}

	int CSprite::GetEditorSize(UEngine& engine) const
	{
		return bIsRequiredByOtherComponent ? 33 : 180 + engine.CurrentScene()->GridSize();
	}

	void CSprite::OnComponentAdd(UEntity& entity) 
	{ 
	}

	void CSprite::OnComponentRemove(UEntity& entity) 
	{ 
	}

	void uei::CSprite::LoadComponent(UEngine& engine, std::istream& in)
	{
		std::string flipX;
		std::string flipY;
		std::string spriteName;
		
		in >> spriteName >> flipX >> flipY;
		
		if (spriteName != EMPTY)
		{
			spriteAsset = new SpriteAsset(&engine.Assets()->GetSpriteAsset(spriteName));
		}
		bFlipX = flipX == "1" ? true : false;
		bFlipY = flipY == "1" ? true : false;
	}

	std::string CSprite::Save() const
	{
		return (spriteAsset == nullptr ? EMPTY : spriteAsset->AssetName) + " " + (bFlipX ? "1" : "0") + " " + (bFlipY ? "1" : "0");
	}
}