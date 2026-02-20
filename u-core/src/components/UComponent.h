#pragma once
#include <memory>
#include <string>
#include <functional>
#include <typeindex>

namespace uei
{
	class UComponent
	{
		friend class UEntity;
		friend class UEditorScene;

	public:
		UComponent();

		virtual std::string ComponentName() const = 0;
		void SetRequiredByOtherComponent(bool value) { bIsRequiredByOtherComponent = value; }

		virtual UComponent* Clone() const = 0;

		virtual void LoadComponent(class UEngine& engine, std::istream & in) = 0;
		virtual std::string Save() const = 0;
	
	protected:
		bool bCanRemove;
		bool bIsRequiredByOtherComponent;

		virtual void OnShowEditor(class UEngine& engine, std::function<void()> onRemove) = 0;
		virtual int GetEditorSize(class UEngine& engine) const = 0;

	private:
		virtual void ShowEditor(class UEngine& engine, std::function<void()> onRemove);

		virtual void OnComponentAdd(class UEntity& entity) = 0;
		virtual void OnComponentRemove(class UEntity& entity) = 0;
	};
}