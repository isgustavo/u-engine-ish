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

	//CSprite::CSprite(const sf::Texture& texture, const int x, const int y, const int width, const int height, const sf::Vector2f scale)
	//	: UComponent()
	//{
	//	sprite = new sf::Sprite(texture, sf::IntRect({ x, y }, { width, height }));
	//	sprite->scale(scale);
	//}

	CSprite::CSprite(const sf::Texture& texture, uei::SpriteData inData, bool flipX)
	{
		data = inData;
		sprite = new sf::Sprite(texture, sf::IntRect({ data.x, data.y }, { data.width, data.height }));
		sprite->setScale(sf::Vector2f((flipX ? -1 : 1) * sprite->getTextureRect().size.x, sprite->getTextureRect().size.y));
		bFlipX = flipX;
	}

	//CSprite::CSprite(std::string inSpriteName)
	//	: UComponent()
	//{
	//	spriteName = inSpriteName;
	//	sprite = nullptr;
	//}

	CSprite::~CSprite()
	{
		delete sprite;
		sprite = nullptr;
	}

	void CSprite::SetScale(int gridSize)
	{
		sprite->setScale(sf::Vector2f((bFlipX ? -1 : 1) * (gridSize / sprite->getTextureRect().size.x), gridSize / sprite->getTextureRect().size.y));
	}

	void CSprite::ShowEditor(UEngine& engine, bool bIsNew)
	{
		if (bIsNew)
		{
			ImGui::BeginChild(
				"Sprite",
				ImVec2(0, 100 + engine.CurrentScene()->GridSize()),
				true
			);

			ImGui::Text("Sprite");
			ImGui::SameLine();

			auto* assets = engine.Assets();
			const auto& allSprites = assets->SpritesNames();

			int selectedIndex = -1;

			for (int i = 0; i < allSprites.size(); i++)
			{
				if (allSprites[i] == data.name)
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
						data = assets->Sprite(allSprites[i]);
						sprite = new sf::Sprite(assets->GetTexture(data.textureName), sf::IntRect({ data.x, data.y }, { data.width, data.height }));
					}
				}
				ImGui::EndCombo();
			}

			if (sprite != nullptr)
			{
				ImGui::Checkbox("FlipX", &bFlipX);
				//std::cout << (bInvert ? -1 : 1) * (engine.CurrentScene()->GridSize() / sprite->getTextureRect().size.x) << std::endl;
				//sprite->setScale(sf::Vector2f((bInvert ? -1 : 1) * (engine.CurrentScene()->GridSize() / sprite->getTextureRect().size.x), engine.CurrentScene()->GridSize() / sprite->getTextureRect().size.y));

				/*data = assets->Sprite(spriteName);
				if (sprite == nullptr)
				{
					sprite = new sf::Sprite(assets->GetTexture(data.textureName), sf::IntRect({ data.x, data.y }, { data.width, data.height }));
					sprite->scale(sf::Vector2f(10.f, 10.f));
				}*/
				ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
				//sf::IntRect r = sprite->getTextureRect();
				sf::Vector2u size = sprite->getTexture().getSize();

				ImGui::Spacing();
				ImGui::Text(data.name.c_str());
				ImGui::Spacing();

				if (bFlipX)
				{
					ImVec2 uv0(
						(float)(data.x + data.width) / size.x,
						(float)data.y / size.y
					);

					ImVec2 uv1(
						(float)data.x / size.x,
						(float)(data.y + data.height) / size.y
					);

					ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
				}
				else 
				{
					ImVec2 uv0(
						(float)data.x / size.x,
						(float)data.y / size.y);

					ImVec2 uv1(
						(float)(data.x + data.width) / size.x,
						(float)(data.y + data.height) / size.y);

					ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
				}
			}

			ImGui::EndChild();
			ImGui::Spacing();
		}
	}

	void CSprite::LoadComponent(UEngine& engine, std::istream& in)
	{
		std::string spriteName;
		std::string flipValue;
		in >> spriteName >> flipValue;
		bFlipX = flipValue == "1" ? true : false;
		data = engine.Assets()->Sprite(spriteName);
		sprite = new sf::Sprite(engine.Assets()->GetTexture(data.textureName), sf::IntRect({ data.x, data.y }, { data.width, data.height }));
	}

	std::string CSprite::Save() const
	{
		return data.name + " " + (bFlipX ? "1" : "0");
	}
}

