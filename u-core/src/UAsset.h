#pragma once
#include "entities/UEntity.h"
#include <string>
#include <map>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

namespace uei
{
	enum EMovement
	{
		NONE,
		UP,
		LEFT,
		DOWN,
		RIGHT,
		UP_LEFT,
		DOWN_LEFT,
		DOWN_RIGHT,
		UP_RIGHT
	};

	inline std::string MovementToString(EMovement movement) 
	{
		switch (movement)
		{
		case EMovement::UP:
			return "UP";
		case EMovement::LEFT:
			return "LEFT";
		case EMovement::DOWN:
			return "DOWN";
		case EMovement::RIGHT:
			return "RIGHT";
		case EMovement::UP_LEFT:
			return "UP_LEFT";
		case EMovement::DOWN_LEFT:
			return "DOWN_LEFT";
		case EMovement::DOWN_RIGHT:
			return "DOWN_RIGHT";
		case EMovement::UP_RIGHT:
			return "UP_RIGHT";
		default:
			return "NONE";
		}
	}

	inline EMovement StringToMovement(std::string movement) 
	{
		if (movement == "UP")
			return EMovement::UP;
		if (movement == "LEFT")
			return EMovement::DOWN;
		if (movement == "DOWN")
			return EMovement::UP;
		if (movement == "RIGHT")
			return EMovement::RIGHT;
		if (movement == "UP_LEFT")
			return EMovement::UP_LEFT;
		if (movement == "DOWN_LEFT")
			return EMovement::DOWN_LEFT;
		if (movement == "DOWN_RIGHT")
			return EMovement::DOWN_RIGHT;
		if (movement == "UP_RIGHT")
			return EMovement::UP_RIGHT;
		return EMovement::NONE;
	}

	class SpriteAsset
	{
	public:
		SpriteAsset() = default;
		SpriteAsset(std::string assetName, std::string textureName, int x, int y, int width, int height) :
			AssetName(assetName), TextureName(textureName), X(x), Y(y), Width(width), Height(height)
		{
		}
		SpriteAsset(const SpriteAsset* spriteAsset) :
			AssetName(spriteAsset->AssetName), TextureName(spriteAsset->TextureName), X(spriteAsset->X), Y(spriteAsset->Y), 
			Width(spriteAsset->Width), Height(spriteAsset->Height)
		{
		}

		const std::string AssetName;
		const std::string TextureName;
		const int X, Y, Width, Height;
	};

	class AnimationAsset : public SpriteAsset
	{
	public:
		AnimationAsset() = default;
		AnimationAsset(std::string assetName, std::string textureName, int x, int y, int width, int height, int frame, float speed) :
			SpriteAsset(assetName, textureName, x, y, width, height), Frame(frame), Speed(speed)
		{
		}

		AnimationAsset(const AnimationAsset& animationAsset) :
			SpriteAsset(animationAsset.AssetName, animationAsset.TextureName, animationAsset.X, animationAsset.Y,
				animationAsset.Width, animationAsset.Height), Frame(animationAsset.Frame), Speed(animationAsset.Speed)
		{
		}

		int Frame;
		float Speed;
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
		const std::vector<std::string>& SpriteNames() const { return spriteNames; };

		const uei::AnimationAsset& GetAnimation(const std::string& animationName) const;
		const std::unordered_map<std::string, uei::AnimationAsset>& Animations() { return animations; }
		const std::vector<std::string>& AnimationNames() const { return animationNames; }

		const uei::UEntity& GetPrefab(const std::string& prefabName) const;
		void RemovePrefab(const std::string& prefabName);
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
		void AddAnimation(const std::string& name, const std::string& textureName, const int x, const int y, const int width, const int height, const int frames, const float speed);
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