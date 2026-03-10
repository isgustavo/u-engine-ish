#include "SDrawSystem.h"
#include "components/CTransform.h"
#include "components/CSprite.h"
#include <components/CStaticDraw.h>

namespace uei
{
	SDrawSystem::SDrawSystem(bool isEditor)
	{
		bIsEditor = isEditor;
	}

	void uei::SDrawSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		std::vector<CTransform*> transforms;
		std::vector<CSprite*> sprites;

		for (auto& e : entities)
		{
			auto* cTransform = e->GetComponent<uei::CTransform>();
			auto* cSprite = e->GetComponent<uei::CSprite>();
			auto* cStaticDraw = e->GetComponent<CStaticDraw>();

			if (cTransform == nullptr || cSprite == nullptr || (cStaticDraw != nullptr && !bIsEditor)) continue;

			transforms.push_back(cTransform);
			sprites.push_back(cSprite);
		}

		sf::VertexArray vertexArray = sf::VertexArray(sf::PrimitiveType::Triangles, transforms.size() * 6);
		float tileSize = engine.CurrentScene()->GridSize();
		for (int i = 0; i < transforms.size(); i++)
		{
			const sf::Vector2f p = transforms[i]->Position();			

			float left = p.x;
			float right = p.x + tileSize;

			float top = p.y;
			float bottom = p.y + tileSize;

			vertexArray[(i * 6) + 0].position = sf::Vector2f(left, top);
			vertexArray[(i * 6) + 1].position = sf::Vector2f(right, top);
			vertexArray[(i * 6) + 2].position = sf::Vector2f(left, bottom);

			vertexArray[(i * 6) + 3].position = sf::Vector2f(left, bottom);
			vertexArray[(i * 6) + 4].position = sf::Vector2f(right, top);
			vertexArray[(i * 6) + 5].position = sf::Vector2f(right, bottom);

			if (sprites[i]->GetSpriteAsset() != nullptr)
			{
				const uei::SpriteAsset* spriteAsset = sprites[i]->GetSpriteAsset();

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

		sf::RenderStates states;
		states.texture = &engine.Assets()->GetTexture("pacman_map.png");
		engine.RenderWindow().draw(vertexArray, states);
	}
}