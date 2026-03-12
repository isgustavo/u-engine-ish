#include "SDrawSystem.h"

#include "components/CTransform.h"
#include "components/CSprite.h"
#include "components/CStaticDraw.h"

namespace uei
{
	SDrawSystem::SDrawSystem()
	{
	}

	void SDrawSystem::Update(UEngine& engine, std::vector<std::unique_ptr<uei::UEntity>>& entities)
	{
		const std::string STATIC = "S_";

		transformMap.clear();
		spriteMap.clear();

		std::unordered_map<std::string, bool> dirtyMap;

		for (auto& e : entities)
		{
			auto* cTransform = e->GetComponent<CTransform>();
			auto* cSprite = e->GetComponent<CSprite>();
			auto* cStaticDraw = e->GetComponent<CStaticDraw>();

			if (cTransform == nullptr || cSprite == nullptr) continue;

			const SpriteAsset* spriteAsset = cSprite->GetSpriteAsset();
			const std::string& key = (cStaticDraw != nullptr ? STATIC : "") + spriteAsset->TextureName;

			if (cSprite->IsDirty())
			{
				dirtyMap[key] = true;
				cSprite->SetDirty(false);
			}

			transformMap[key].push_back(cTransform);
			spriteMap[key].push_back(cSprite);
		}

		for (auto& [key, value] : transformMap)
		{
			if (!dirtyMap[key]) continue;

			std::cout << key << std::endl;

			std::vector<CTransform*> transforms = value;
			std::vector<CSprite*> sprites = spriteMap[key];

			vertexArrayMap[key] = sf::VertexArray(sf::PrimitiveType::Triangles, transforms.size() * 6);
			sf::VertexArray& vertexArray = vertexArrayMap[key];
			for (int i = 0; i < transforms.size(); i++)
			{
				const sf::Vector2f p = transforms[i]->Position();

				float left = p.x;
				float right = p.x + engine.CurrentScene()->GridSize();

				float top = p.y;
				float bottom = p.y + engine.CurrentScene()->GridSize();

				vertexArray[(i * 6) + 0].position = sf::Vector2f(left, top);
				vertexArray[(i * 6) + 1].position = sf::Vector2f(right, top);
				vertexArray[(i * 6) + 2].position = sf::Vector2f(left, bottom);

				vertexArray[(i * 6) + 3].position = sf::Vector2f(left, bottom);
				vertexArray[(i * 6) + 4].position = sf::Vector2f(right, top);
				vertexArray[(i * 6) + 5].position = sf::Vector2f(right, bottom);

				CSprite* cSprite = sprites[i];
				if (cSprite->GetSpriteAsset() != nullptr)
				{
					const uei::SpriteAsset* spriteAsset = cSprite->GetSpriteAsset();

					left = spriteAsset->X;
					right = spriteAsset->X + spriteAsset->Width;
					top = spriteAsset->Y;
					bottom = spriteAsset->Y + spriteAsset->Height;

					if (cSprite->FlipX())
						std::swap(left, right);

					if (cSprite->FlipY())
						std::swap(top, bottom);

					vertexArray[(i * 6) + 0].texCoords = { left,  top };
					vertexArray[(i * 6) + 1].texCoords = { right, top };
					vertexArray[(i * 6) + 2].texCoords = { left,  bottom };

					vertexArray[(i * 6) + 3].texCoords = { left,  bottom };
					vertexArray[(i * 6) + 4].texCoords = { right, top };
					vertexArray[(i * 6) + 5].texCoords = { right, bottom };
				}
			}
		}

		for (auto& [key, value] : vertexArrayMap)
		{
			sf::RenderStates states;

			if(key.substr(0, 2) == STATIC)
				states.texture = &engine.Assets()->GetTexture(key.substr(2, key.length() - 2));
			else 
				states.texture = &engine.Assets()->GetTexture(key);

			engine.RenderWindow().draw(value, states);
		}
	}
}