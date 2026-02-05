#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	struct SpriteData
	{
		std::string name;
		std::string textureName;
		int x, y, width, height;
		float scaleX, scaleY;
	};
	class UAsset
	{
	public:
		UAsset();
		void LoadFromFile(const std::string& path);

		const sf::Texture& GetTexture(const std::string& textureName) const;
		const std::unordered_map<std::string, sf::Texture>& Textures() { return textures; }

		uei::SpriteData& Sprite(const std::string& spriteName);
		const std::vector<std::string>& SpritesNames() const;
		
		//const Animation& GetAnimation(const std::string& animationName) const;
		const sf::Font& GetFont(const std::string& fontName) const;
		//const std::map<std::string, sf::Texture>& GetTexture() const;
		//const std::map<std::string, Animation>& GetAniamtions() const;
		//const std::map<std::string, sf::Sound>& GetSound() const;
	private:
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, uei::SpriteData> sprites;
		std::vector<std::string> spritesNames;
		std::map<std::string, sf::Font> fonts;
		//std::map<std::string, Animation> animations;

		void AddTexture(const std::string& name, const std::string& path);
		void AddFont(const std::string& name, const std::string& path);
		void AddSprite(const std::string& name, const std::string& path, const int x, const int y, const int width, const int height);
	};
}