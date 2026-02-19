#pragma once
#include <memory>
#include <string>

namespace uei
{
	class UComponent
	{
	public:
		UComponent();

		virtual UComponent* Clone() const = 0;
		virtual std::string ComponentName() const = 0;
		virtual void ShowEditor(class UEngine& inEngine, bool bIsNew) = 0;
		virtual void LoadComponent(UEngine& engine, std::istream & in) = 0;
		virtual std::string Save() const = 0;
	};
}