#pragma once
#include "UEngine.h"

namespace uei
{
	class UEditorScene : public UScene
	{
	public:
		UEditorScene(UEngine& inEngine);
		~UEditorScene();

		virtual void OnStart() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
		virtual void OnMouseLeft() override;
		virtual void OnMouseRight() override;

	private:
		//char sceneName[128] = "";
		class UEntity* prefab;

		char editorNameBuffer[128] = "";
		bool bSceneLoaded;
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
		int newAnimationSpeed = 0;
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

		//void SaveAsset();
		//void SaveUpdatePrefab();
		//void SavePrefab();
		void CancelPrefabSelected();

		void ClearEditor();
		
	};
}