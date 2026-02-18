#include "UAsset.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>
#include <UEngine.h>

namespace uei
{
	UAsset::UAsset()
	{
		//const std::string TEXTURE("Texture");
		//const std::string ANIMATION("Animation");
		//const std::string FONT("Font");
		//const std::string SPRITE("Sprite");
		//const std::string ASSETS_PATH("Assets/");

		textures.clear();
		AddTextures();
		sprites.clear();
		spriteNames.clear();

		std::ifstream file("Assets/Assets.txt");
		std::string str;

		while (file.good())
		{
			file >> str;
			if (str == TEXTURE)
			{
				std::string name, texturePath;
				file >> name >> texturePath;
				std::cout << name << texturePath << std::endl;
				//AddTexture(name, ASSETS_PATH + texturePath);
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
			str = "";
		}
	}
	void UAsset::AddTextures()
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
					std::cout << "ADD TEXTURE" << t.path().filename().string() << std::endl;
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
	uei::SpriteData& UAsset::Sprite(const std::string& spriteName)
	{
		auto it = sprites.find(spriteName);
		return it->second;
	}
	const std::vector<std::string>& UAsset::SpritesNames() const
	{
		return spriteNames;
	}
	uei::AnimationData& UAsset::Animation(const std::string& animationName)
	{
		auto it = animations.find(animationName);
		return it->second;
	}
}