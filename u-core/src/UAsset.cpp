#include "UAsset.h"

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

	void uei::UAsset::LoadFromFile(const std::string& inPath) // ToDo remove inPath param
	{
		const std::string TEXTURE("Texture");
		const std::string ANIMATION("Animation");
		const std::string FONT("Font");
		const std::string SPRITE("Sprite");
		const std::string ASSETS_PATH("Assets/");

		textures.clear();
		sprites.clear();
		spritesNames.clear();

		std::ifstream file("Assets/Assets.txt");
		std::string str;
		//std::string name, path;
		//int x, y;
		//int width, height;

		while (file.good())
		{
			file >> str;
			if (str == TEXTURE)
			{
				std::string name, texturePath;
				file >> name >> texturePath;
				std::cout << name << texturePath << std::endl;
				AddTexture(name, ASSETS_PATH + texturePath);
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
				std::string texture;
				//size_t frames, speed;
				//file >> name >> texture >> frames >> speed;
				//AddAnimation(name, texture, frames, speed);
			}
			else if (str == FONT)
			{
				//file >> name >> path;
				//AddFont(name, path);
			}
		}

	}
	void UAsset::AddTexture(const std::string& name, const std::string& path)
	{
		sf::Texture texture = sf::Texture();
		if (!texture.loadFromFile(path))
		{
			std::cerr << "Could not load texture file:" << path << std::endl;
		}
		else 
		{
			textures.emplace(name, texture);
		}
	}
	void UAsset::AddSprite(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height)
	{
		auto& texture = GetTexture(textureName);
		spritesNames.push_back(name);
		//sprites.emplace(name, sf::Sprite(texture, sf::IntRect({ x, y }, { width, height })));
		sprites.emplace(name, SpriteData{ name, textureName, x, y, width, height });
	}
	/*void UAsset::AddAnimation()
	{
		animations[name] = Animation(name, textrue, frames, speed)
	}*/
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
		assert(it != textures.end());
		return it->second;
	}
	uei::SpriteData& UAsset::Sprite(const std::string& spriteName)
	{
		auto it = sprites.find(spriteName);
		assert(it != sprites.end());
		return it->second;
	}
	const std::vector<std::string>& UAsset::SpritesNames() const
	{
		return spritesNames;
	}
}