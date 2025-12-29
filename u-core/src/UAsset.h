#pragma once
#include <string>
#include <map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	class UAsset
	{
	public:
		void LoadFromFile(const std::string& path);

		const sf::Texture& GetTexture(const std::string& textureName) const;
		//const Animation& GetAnimation(const std::string& animationName) const;
		const sf::Font& GetFont(const std::string& fontName) const;
		//const std::map<std::string, sf::Texture>& GetTexture() const;
		//const std::map<std::string, Animation>& GetAniamtions() const;
		//const std::map<std::string, sf::Sound>& GetSound() const;
	private:
		std::map<std::string, sf::Texture> textures;
		//std::map<std::string, Animation> animations;
		std::map<std::string, sf::Font> fonts;
		//std::map<std::string, Animation> animations;

		void AddTexture(const std::string& name, const std::string& path);
		void AddFont(const std::string& name, const std::string& path);
	};
}