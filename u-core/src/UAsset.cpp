#include "UAsset.h"
#include "UEngine.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>

namespace uei
{
	UAsset::UAsset()
	{
		
	}
	void UAsset::Load(UEngine& engine)
	{
		textures.clear();

		sprites.clear();
		spriteNames.clear();

		animations.clear();
		animationNames.clear();

		LoadTextures();

		uei::UEntity* prefab = nullptr;

		std::ifstream file("Assets/Assets.txt");
		std::string str;

		while (file.good())
		{
			file >> str;
			if (str == TEXTURE)
			{
				std::string name, texturePath;
				file >> name >> texturePath;
			}
			else if (str == SPRITE)
			{
				std::string name, textureName;
				int x, y, width, height;
				file >> name >> textureName >> x >> y >> width >> height;
				std::cout << name << textureName << std::endl;
				AddSprite(name, textureName, x, y, width, height);
			}
			else if (str == ANIMATION)
			{
				std::string name, textureName;
				int x, y, width, height, frames, speed;
				file >> name >> textureName >> x >> y >> width >> height >> frames >> speed;
				AddAnimation(name, textureName, x, y, width, height, frames, speed);
			}
			//else if (str == FONT)
			//{
			//	//file >> name >> path;
			//	//AddFont(name, path);
			//}
			else if (str == PREFAB)
			{
				if (prefab != nullptr)
				{
					prefabs.emplace(prefab->name, prefab->Clone());
					prefab = nullptr;
				}
				std::string prefabName;
				file >> prefabName;

				prefab = new UEntity(0, prefabName);
			}
			else if (str == COMPONENT)
			{
				std::string componentName;
				file >> componentName;
				if (componentName.empty()) continue;
				auto* newComponent = UEditor::Create(componentName);
				newComponent->LoadComponent(engine, file);
				prefab->AddComponent(newComponent);
			}

			str = "";
		}

		if (prefab != nullptr)
		{
			prefabs.emplace(prefab->name, prefab->Clone());
			prefab = nullptr;
		}
	}
	void UAsset::Save()
	{
		std::string filePath = "Assets/Assets.txt";
		if (std::filesystem::exists(filePath))
		{
			std::string content;
			content.clear();

			for (const auto& [key, value] : sprites)
			{
				content += SPRITE + " " + key + " " + value.textureName + " " +
					std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.width) + " " + std::to_string(value.height) + "\n";
			}

			//auto& allAsstTypes = engine.Assets()->AllAssetTypes();
			//if (allAsstTypes[assetTypeSelectedIndex] == SPRITE)
			//{
			//	content += SPRITE + " " + newName + " " + engine.Assets()->AllTextures()[assetTextureSelectedIndex] + " " +
			//		std::to_string(newSpriteX) + " " + std::to_string(newSpriteY) + " " + std::to_string(newSpriteWidth) + " " + std::to_string(newSpriteHeight) + "\n";
			//}

			for (const auto& [key, value] : animations)
			{
				content += ANIMATION + " " + key + " " + value.textureName + " " +
					std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.width) + " " + std::to_string(value.height) + " " +
					std::to_string(value.frame) + " " + std::to_string(value.speed) + "\n";
			}

			for (const auto& [key, value] : prefabs)
			{
				content += PREFAB + " " + value->Name() + "\n";

				for (const auto& [c_key, c_value] : value->Components())
				{
					content += COMPONENT + " " + c_value->ComponentName() + " " + c_value->Save() + "\n";
				}
			}

			//if (allAsstTypes[assetTypeSelectedIndex] == ANIMATION)
			//{
			//	content += ANIMATION + " " + newName + " " + engine.Assets()->AllTextures()[assetTextureSelectedIndex] + " " +
			//		std::to_string(newSpriteX) + " " + std::to_string(newSpriteY) + " " + std::to_string(newSpriteWidth) + " " + std::to_string(newSpriteHeight) + " " +
			//		std::to_string(newAnimationFrame) + " " + std::to_string(newAnimationSpeed) + "\n";
			//}

			std::ofstream out(filePath);
			out << content;
			out.close();
		}
	}
	void UAsset::LoadTextures()
	{
		for (const auto& t : std::filesystem::directory_iterator("Assets/Textures"))
		{
			if (t.is_regular_file() && t.path().extension() == ".png")
			{
				sf::Texture texture = sf::Texture();
				if (!texture.loadFromFile(t.path()))
				{
					std::cerr << "Could not load texture file:" << t.path() << std::endl;
				}
				else
				{
					textures.emplace(t.path().filename().string(), texture);
					AllTextures().push_back(t.path().filename().string());
				}
			}
		}
	}
	void UAsset::AddSprite(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height)
	{
		auto& texture = GetTexture(textureName);
		spriteNames.push_back(name);
		sprites.emplace(name, SpriteData{ name, textureName, x, y, width, height });
	}
	void UAsset::AddAnimation(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height, const int frames, const int speed)
	{
		auto& texture = GetTexture(textureName);
		animationNames.push_back(name);
		animations.emplace(name, AnimationData{ name, textureName, x, y, width, height, frames, speed });
	}
	void UAsset::AddPrefab(const std::string& name, uei::UEntity* prefab)
	{
		prefabs.emplace(prefab->name, prefab);
	}
	void UAsset::AddFont(const std::string& name, const std::string& path)
	{
		fonts[name] = sf::Font();
		if (!fonts[name].openFromFile(path))
		{
			std::cerr << "Could not load font file:" << path << std::endl;
			fonts.erase(name);
		}
	}
	const sf::Texture& UAsset::GetTexture(const std::string& textureName) const
	{
		auto it = textures.find(textureName);
		return it->second;
	}
	const uei::SpriteData& UAsset::Sprite(const std::string& spriteName) const
	{
		auto it = sprites.find(spriteName);
		return it->second;
	}
	const std::vector<std::string>& UAsset::SpritesNames() const
	{
		return spriteNames;
	}
	const uei::AnimationData& UAsset::Animation(const std::string& animationName) const
	{
		auto it = animations.find(animationName);
		return it->second;
	}
}