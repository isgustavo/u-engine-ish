#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

namespace uei
{
	class UComponent;
	class USystem;

	class UEditor
	{
	public:
		using ComponentFunction = std::function<UComponent* ()>;
		static void RegisterComponent(const std::string& typeName, ComponentFunction cf);
		static UComponent* CreateComponent(const std::string& typeName);
		static const std::vector<std::string>& AllComponents();

		using SystemFunction = std::function<USystem* ()>;
		static void RegisterSystem(const std::string& typeName, SystemFunction sf);
		static USystem* CreateSystem(const std::string& typeName);
		static const std::vector<std::string>& AllSystems();
	};
}

#define REGISTER_COMPONENT(T) static bool _registered_##T = []() { uei::UEditor::RegisterComponent(#T, []() { return new uei::T(); }); return true; }()

#define REGISTER_SYSTEM(T) static bool _registered_##T = []() { uei::UEditor::RegisterSystem(#T, []() { return new uei::T(); }); return true; }()