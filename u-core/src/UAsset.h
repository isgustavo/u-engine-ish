#pragma once
#include <string>
#include <map>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	class SpriteData
	{
	public:
		SpriteData() = default;
		SpriteData(std::string inName, std::string inTextureName, int inX, int inY, int inWidth, int inHeight) :
			name(inName), textureName(inTextureName), x(inX), y(inY), width(inWidth), height(inHeight)
		{
		}

		std::string name;
		std::string textureName;
		int x{}, y{}, width{}, height{};
	};

	class AnimationData : public SpriteData
	{
	public:
		AnimationData(std::string inName, std::string inTextureName, int inX, int inY, int inWidth, int inHeight, int inFrame, int inSpeed) :
			SpriteData(inName, inTextureName, inX, inY, inWidth, inHeight), frame(inFrame), speed(inSpeed)
		{
		}

		int frame{}, speed{};
	};

	class UAsset
	{
	public:
		UAsset();

		const sf::Texture& GetTexture(const std::string& textureName) const;
		const std::unordered_map<std::string, sf::Texture>& Textures() { return textures; }

		uei::SpriteData& Sprite(const std::string& spriteName);
		const std::vector<std::string>& SpritesNames() const;
		std::unordered_map<std::string, uei::SpriteData> Sprites() { return sprites; }

		uei::AnimationData& Animation(const std::string& animationName);
		std::unordered_map<std::string, uei::AnimationData> Animations() { return animations; }
		//const Animation& GetAnimation(const std::string& animationName) const;
		const sf::Font& GetFont(const std::string& fontName) const;
		//const std::map<std::string, sf::Texture>& GetTexture() const;
		//const std::map<std::string, Animation>& GetAniamtions() const;
		//const std::map<std::string, sf::Sound>& GetSound() const;
		std::vector<std::string>& AllAssetTypes() 
		{ 
			if (allAssetTypesName.size() == 0)
			{
				allAssetTypesName.push_back(" ");
				allAssetTypesName.push_back("Sprite");
				allAssetTypesName.push_back("Animation");
				allAssetTypesName.push_back("Font");
			}
			return allAssetTypesName; 
		}
		std::vector<std::string>& AllTextures() { return allTexturesName; }

	private:
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, uei::SpriteData> sprites;
		std::unordered_map<std::string, uei::AnimationData> animations;
		std::vector<std::string> spriteNames;
		std::vector<std::string> animationNames;
		std::map<std::string, sf::Font> fonts;
		//std::map<std::string, Animation> animations;

		void AddTextures();
		void AddFont(const std::string& name, const std::string& path);
		void AddSprite(const std::string& name, const std::string& path, const int x, const int y, const int width, const int height);
		void AddAnimation(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height, const int frames, const int speed);

		std::vector<std::string> allAssetTypesName;
		std::vector<std::string> allTexturesName;
	};


}