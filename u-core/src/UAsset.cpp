#include "UAsset.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>

namespace uei
{
	void uei::UAsset::LoadFromFile(const std::string& inPath)
	{
		const std::string TEXTURE("Texture");
		const std::string ANIMATION("Animation");
		const std::string FONT("Font");

		std::ifstream file(inPath);
		std::string str;
		std::string name, path;
		while (file.good())
		{
			file >> str;
			if (str == TEXTURE)
			{
				file >> name >> path;
				AddTexture(name, path);
			} 
			else if (str == ANIMATION)
			{
				std::string texture;
				size_t frames, speed;
				file >> name >> texture >> frames >> speed;
				//AddAnimation(name, texture, frames, speed);
			}
			else if (str == FONT)
			{
				file >> name >> path;
				AddFont(name, path);
			}
		}

	}
	void UAsset::AddTexture(const std::string& name, const std::string& path)
	{
		textures[name] = sf::Texture();
		if (!textures[name].loadFromFile(path))
		{
			std::cerr << "Could not load texture file:" << path << std::endl;
			textures.erase(name);
		}
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
}
