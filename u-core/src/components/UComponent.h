#pragma once
#include <memory>
#include <string>
#include <sstream>
#include <functional>
#include <typeindex>

template<typename... Args>
std::string Serialize(const Args&... args)
{
	std::ostringstream ss;
	((ss << args << " "), ...);
	return ss.str();
}

template<typename... Args>
void Deserialize(std::istream& in, Args&... args)
{
	((in >> args), ...);
}

namespace uei
{
	class UComponent
	{
		friend class UEntity;
		friend class UEditorScene;

	public:
		UComponent(bool canRemove = true);

		bool IsRequiredByOtherComponent() const;
		void SetRequiredByOtherComponent(bool value);

		virtual UComponent* Clone() = 0;

		virtual void Start(class UEngine& engine);

		virtual std::string ComponentName() const = 0;
		inline virtual void LoadComponent(std::istream& in) {}
		inline virtual std::string SaveComponent() const { return std::string(); }

	protected:
		bool bCanRemove;
		int requiredByOtherComponent;

		virtual int GetEditorSize() const { return 33; }
		inline virtual void OnShowEditor(class UEngine& engine) {}
		
		inline virtual void OnComponentAdd(class UEntity& entity) {}
		inline virtual void OnComponentRemove(class UEntity& entity) {}

	private:
		virtual void ShowEditor(class UEngine& engine, std::function<void()> onRemove);

	};
}