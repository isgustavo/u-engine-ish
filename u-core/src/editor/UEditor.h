#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <components/UComponent.h>

namespace uei
{
	class UEditor
	{
	public:
		using ComponentFunction = std::function<UComponent* ()>;
		static void Register(const std::string& typeName, ComponentFunction cf);
		static UComponent* Create(const std::string& typeName);
		static const std::vector<std::string>& AllComponents();
	};
}

#define REGISTER_COMPONENT(T) static bool _registered_##T = []() { uei::UEditor::Register(#T, []() { return new uei::T(); }); return true; }()