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
		virtual std::string Editor_ComponentName() const = 0;
		virtual void Editor_Show(class UEngine& inEngine, bool bIsNew) = 0;
		virtual void Editor_Load(std::istream & in) = 0;
		virtual std::string Editor_Save() const = 0;
	};
}