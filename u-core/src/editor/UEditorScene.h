#pragma once
#include "UEngine.h"

namespace uei
{
	class UEditorScene : public UScene
	{
	public:
		UEditorScene(UEngine& inEngine, std::string levelName);
		~UEditorScene();

		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnDraw() override;
		virtual void OnMouseLeft() override;
		virtual void OnMouseRight() override;

	private:
		class UEntity* prefab;

		char editorNameBuffer[128] = "";
		bool bSceneLoaded;
		std::string levelNameLoaded;
		bool bNewScene;
		bool bMainTab;
		bool bShowNewAsset;
		bool bShowNewPrefab;
		bool bShowEditPrefab;
		bool bPrefabSelected;
		int newSceneGridColumn = 5;
		int newSceneGridRow = 5;
		int newSceneGridSize = 90;
		int assetTypeSelectedIndex;
		int assetTextureSelectedIndex;
		int newSpriteX;
		int newSpriteY;
		int newSpriteWidth;
		int newSpriteHeight;
		int newAnimationFrame = 0;
		float newAnimationSpeed = 0;
		int currentAnimationFrame = 0;
		float currentAnimationDeltaTime = .0f;

		void ShowSceneTab();
		void ShowOpenCreateTab();
		void ShowSceneOpenTab();

		void ShowAssetsTab();
		void ShowNewAssetTab();
		void ShowPrefabComponent();
		void ShowAssetsGallery();

		void ShowPrefabTab();
		void ShowNewPrefabTab();
		void ShowEditPrefabTab();
		void ShowPrefabGallery();

		void CreateScene();
		void LoadScene();
		void SaveScene();
		void CloseScene();

		void AddImGui(const AnimationAsset* animationData, float thumbnailSize = 64.0f);
		void AddImGui(const std::string& textureName, int x, int y, int width, int height, float speed, int frame, float thumbnailSize = 64.0f);
		void AddImGui(const SpriteAsset* spriteData, bool flipX, bool flipY, float thumbnailSize = 64.0f);
		void AddImGui(const std::string& textureName, int x, int y, int width, int height, bool flipX, bool flipY, float thumbnailSize = 64.0f);

		//void SaveAsset();
		//void SaveUpdatePrefab();
		//void SavePrefab();
		void CancelPrefabSelected();

		void ClearEditor();
		
	};
}