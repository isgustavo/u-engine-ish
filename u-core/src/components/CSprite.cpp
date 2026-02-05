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

	CSprite::CSprite(const sf::Texture& texture, const int x, const int y, const int width, const int height, const sf::Vector2f scale)
		: UComponent()
	{
		sprite = new sf::Sprite(texture, sf::IntRect({ x, y }, { width, height }));
		sprite->scale(scale);
	}

	CSprite::CSprite(std::string inSpriteName)
		: UComponent()
	{
		spriteName = inSpriteName;
		sprite = nullptr;
	}

	CSprite::~CSprite()
	{
		delete sprite;
		sprite = nullptr;
	}

	void CSprite::Editor_Show(UEngine& inEngine, bool bIsNew)
	{
		if (bIsNew)
		{
			ImGui::BeginChild(
				"Sprite",
				ImVec2(0, 140),
				true
			);

			ImGui::Text("Sprite");
			ImGui::SameLine();

			auto& assets = inEngine.Assets();
			const auto& allSprites = assets.SpritesNames();

			if (ImGui::BeginCombo("##Sprite", " "))
			{
				for (int i = 0; i < allSprites.size(); ++i) {
					const bool isSelected = (editor_selectedIndex == i);
					if (ImGui::Selectable(allSprites[i].c_str(), isSelected))
					{
						editor_selectedIndex = i;
						editor_data = assets.Sprite(allSprites[i]);
						spriteName = editor_data.name;
						sprite = new sf::Sprite(inEngine.Assets().GetTexture(editor_data.textureName), sf::IntRect({ editor_data.x, editor_data.y }, { editor_data.width, editor_data.height }));
						sprite->scale(sf::Vector2f(10.f, 10.f));
					}
				}
				ImGui::EndCombo();
			}

			if (!spriteName.empty())
			{
				editor_data = assets.Sprite(spriteName);
				if (sprite == nullptr)
				{
					sprite = new sf::Sprite(inEngine.Assets().GetTexture(editor_data.textureName), sf::IntRect({ editor_data.x, editor_data.y }, { editor_data.width, editor_data.height }));
					sprite->scale(sf::Vector2f(10.f, 10.f));
				}
				ImTextureID id = (ImTextureID)(intptr_t)sprite->getTexture().getNativeHandle();
				//sf::IntRect r = sprite->getTextureRect();
				sf::Vector2u size = sprite->getTexture().getSize();

				ImGui::Spacing();
				ImGui::Text(editor_data.name.c_str());
				ImGui::Spacing();

				ImVec2 uv0(
					(float)editor_data.x / size.x,
					(float)editor_data.y / size.y);

				ImVec2 uv1(
					(float)(editor_data.x + editor_data.width) / size.x,
					(float)(editor_data.y + editor_data.height) / size.y);

				ImGui::Image(id, ImVec2(64, 64), uv0, uv1);
			}


			ImGui::EndChild();
			ImGui::Spacing();
		}

	}
	void CSprite::Editor_Load(std::istream& in)
	{
		in >> spriteName;
	}

	std::string CSprite::Editor_Save() const
	{
		return spriteName;
	}
}

