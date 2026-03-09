#include "UAsset.h"
#include "UEngine.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>

namespace uei
{
	UAsset::UAsset() { }

	void UAsset::Load(UEngine& engine)
	{
		textures.clear();

		spriteAssets.clear();
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
				int x, y, width, height, frames;
				float speed;
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

			for (const auto& [key, value] : spriteAssets)
			{
				content += SPRITE + " " + key + " " + value.TextureName + " " +
					std::to_string(value.X) + " " + std::to_string(value.Y) + " " + std::to_string(value.Width) + " " + std::to_string(value.Height) + "\n";
			}

			for (const auto& [key, value] : animations)
			{
				content += ANIMATION + " " + key + " " + value.TextureName + " " +
					std::to_string(value.X) + " " + std::to_string(value.Y) + " " + std::to_string(value.Width) + " " + std::to_string(value.Height) + " " +
					std::to_string(value.Frame) + " " + std::to_string(value.Speed) + "\n";
			}

			for (const auto& [key, value] : prefabs)
			{
				content += PREFAB + " " + value->Name() + "\n";

				for (const auto& [c_key, c_value] : value->Components())
				{
					content += COMPONENT + " " + c_value->ComponentName() + " " + c_value->Save() + "\n";
				}
			}

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
		spriteAssets.emplace(name, SpriteAsset{ name, textureName, x, y, width, height });
	}
	void UAsset::AddAnimation(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height, const int frames, const float speed)
	{
		auto& texture = GetTexture(textureName);
		animationNames.push_back(name);
		animations.emplace(name, AnimationAsset{ name, textureName, x, y, width, height, frames, speed });
	}
	void UAsset::AddPrefab(const std::string& name, uei::UEntity* prefab)
	{
		prefab->Name(name);
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
	const uei::SpriteAsset& UAsset::GetSpriteAsset(const std::string& spriteName) const
	{
		auto it = spriteAssets.find(spriteName);
		return it->second;
	}
	const uei::AnimationAsset& UAsset::GetAnimation(const std::string& animationName) const
	{
		auto it = animations.find(animationName);
		return it->second;
	}
	const uei::UEntity& UAsset::GetPrefab(const std::string& prefabName) const
	{
		auto it = prefabs.find(prefabName);
		return *it->second;
	}
	
	void UAsset::RemovePrefab(const std::string& prefabName)
	{
		auto it = prefabs.find(prefabName);
		UEntity* prefab = it->second;
		prefabs.erase(prefabName);
		delete prefab;
		prefab = nullptr;

	}
}