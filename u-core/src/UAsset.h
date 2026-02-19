#pragma once
#include "entities/UEntity.h"
#include <string>
#include <map>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	class SpriteAsset
	{
	public:
		SpriteAsset() = default;
		SpriteAsset(std::string inName, std::string inTextureName, int inX, int inY, int inWidth, int inHeight) :
			name(inName), textureName(inTextureName), x(inX), y(inY), width(inWidth), height(inHeight)
		{
		}

		std::string name;
		std::string textureName;
		int x{}, y{}, width{}, height{};
	};

	class AnimationAsset : public SpriteAsset
	{
	public:
		AnimationAsset(std::string inName, std::string inTextureName, int inX, int inY, int inWidth, int inHeight, int inFrame, int inSpeed) :
			SpriteAsset(inName, inTextureName, inX, inY, inWidth, inHeight), frame(inFrame), speed(inSpeed)
		{
		}

		int frame{}, speed{};
	};

	class UAsset
	{
	public:
		UAsset();
		void Load(UEngine& engine);

		const sf::Texture& GetTexture(const std::string& textureName) const;
		const std::unordered_map<std::string, sf::Texture>& Textures() { return textures; }

		const uei::SpriteAsset& GetSpriteAsset(const std::string& spriteName) const;
		const std::unordered_map<std::string, uei::SpriteAsset>& AllSpriteAssets() { return spriteAssets; }
		const std::vector<std::string>& SpritesNames() const;

		const uei::AnimationAsset& Animation(const std::string& animationName) const;
		const std::unordered_map<std::string, uei::AnimationAsset>& Animations() { return animations; }

		const uei::UEntity& GetPrefab(const std::string& prefabName) const;
		const std::unordered_map<std::string, uei::UEntity*>& Prefabs() { return prefabs; }

		const sf::Font& GetFont(const std::string& fontName) const;

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

		void Save();
		void AddSprite(const std::string& name, const std::string& path, const int x, const int y, const int width, const int height);
		void AddAnimation(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height, const int frames, const int speed);
		void AddPrefab(const std::string& name, uei::UEntity* prefab);

	private:
		std::unordered_map<std::string, sf::Texture> textures;
		std::unordered_map<std::string, uei::SpriteAsset> spriteAssets;
		std::unordered_map<std::string, uei::AnimationAsset> animations;

		//uei::UEntity newPrefab;
		std::unordered_map<std::string, uei::UEntity*> prefabs;

		std::vector<std::string> spriteNames;
		std::vector<std::string> animationNames;

		std::map<std::string, sf::Font> fonts;
		//std::map<std::string, Animation> animations;

		
		void LoadTextures();
		void AddFont(const std::string& name, const std::string& path);
		
		std::vector<std::string> allAssetTypesName;
		std::vector<std::string> allTexturesName;
	};


}