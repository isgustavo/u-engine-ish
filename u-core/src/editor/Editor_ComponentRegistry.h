#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <components/UComponent.h>

namespace uei
{
	class Editor_ComponentRegistry
	{
	public:
		//using ComponentFunction = std::function<std::unique_ptr<UComponent>()>;

		//static void Register(const std::string& inTypeName, ComponentFunction inCf);
		//static std::unique_ptr<UComponent> Create(const std::string& inTypeName);
		//static const std::vector<std::string>& AllComponents();
		using ComponentFunction = std::function<UComponent*()>;

		static void Register(const std::string& typeName, ComponentFunction inCf);
		static UComponent* Create(const std::string& typeName);
		static const std::vector<std::string>& AllComponents();
	};
}

#define REGISTER_COMPONENT(T) static bool _registered_##T = []() { uei::Editor_ComponentRegistry::Register(#T, []() { return new uei::T(); }); return true; }()
//#define REGISTER_COMPONENT(T) static bool _registered_##T = []() { uei::Editor_ComponentRegistry::Register(#T, []() { return std::make_unique<uei::T>(); }); return true; }()
