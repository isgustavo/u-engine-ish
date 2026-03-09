#include "SStaticDrawSystem.h"
#include "components/CSprite.h"
#include "components/CTransform.h"
#include <components/CStaticDraw.h>

namespace uei
{
	SStaticDrawSystem::SStaticDrawSystem()
	{
		bIsDirty = true;
	}

	void SStaticDrawSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		if (!bIsDirty)
		{
			sf::RenderStates states;
			states.texture = &engine.Assets()->GetTexture("pacman_map.png");
			engine.RenderWindow().draw(vertexArray, states);
		}
		else 
		{
			std::vector<CTransform*> transforms;
			std::vector<CSprite*> sprites;

			for (auto& e : entities)
			{
				auto* cTransform = e->GetComponent<CTransform>();
				auto* cSprite = e->GetComponent<CSprite>();
				auto* cStaticDraw = e->GetComponent<CStaticDraw>();

				if (cTransform == nullptr || cSprite == nullptr || cStaticDraw == nullptr) continue;

				transforms.push_back(cTransform);
				sprites.push_back(cSprite);
			}

			vertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, transforms.size() * 6);
			float tileSize = engine.CurrentScene()->GridSize();
			for (int i = 0; i < transforms.size(); i++)
			{
				const sf::Vector2f p = transforms[i]->Position();
				
				float left = sprites[i]->FlipX() ? p.x - tileSize : p.x;
				float right = sprites[i]->FlipX() ? p.x : p.x + tileSize;

				float top = sprites[i]->FlipY() ? p.y - tileSize : p.y;
				float bottom = sprites[i]->FlipY() ? p.y : p.y + tileSize;

				vertexArray[(i * 6) + 0].position = sf::Vector2f(left, top);
				vertexArray[(i * 6) + 1].position = sf::Vector2f(right, top);
				vertexArray[(i * 6) + 2].position = sf::Vector2f(left, bottom);

				vertexArray[(i * 6) + 3].position = sf::Vector2f(left, bottom);
				vertexArray[(i * 6) + 4].position = sf::Vector2f(right, top);
				vertexArray[(i * 6) + 5].position = sf::Vector2f(right, bottom);

				if (sprites[i]->GetSpriteAsset() != nullptr)
				{
					const SpriteAsset* spriteAsset = sprites[i]->GetSpriteAsset();// engine.Assets()->GetSpriteAsset(sprites[i]->SpriteName());

					left = spriteAsset->X;
					right = spriteAsset->X + spriteAsset->Width;
					top = spriteAsset->Y;
					bottom = spriteAsset->Y + spriteAsset->Height;

					if (sprites[i]->FlipX())
						std::swap(left, right);

					if (sprites[i]->FlipY())
						std::swap(top, bottom);

					vertexArray[(i * 6) + 0].texCoords = { left,  top };
					vertexArray[(i * 6) + 1].texCoords = { right, top };
					vertexArray[(i * 6) + 2].texCoords = { left,  bottom };

					vertexArray[(i * 6) + 3].texCoords = { left,  bottom };
					vertexArray[(i * 6) + 4].texCoords = { right, top };
					vertexArray[(i * 6) + 5].texCoords = { right, bottom };
				}
			}

			bIsDirty = false;
		}
	}
}