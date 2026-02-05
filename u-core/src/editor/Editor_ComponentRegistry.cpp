#include "Editor_ComponentRegistry.h"
#include "components/UComponent.h"
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>

namespace uei
{
    static std::unordered_map<std::string, Editor_ComponentRegistry::ComponentFunction>& Components()
    {
        static std::unordered_map<std::string, Editor_ComponentRegistry::ComponentFunction> components;
        return components;
    }
    static std::vector<std::string>& ComponentsVector()
    {
        static std::vector<std::string> components;
        return components;
    }
    void Editor_ComponentRegistry::Register(const std::string& typeName, ComponentFunction inCf)
    {
        auto& map = Components();
        map.emplace(typeName, inCf);
        auto& vector = ComponentsVector();
        vector.push_back(typeName);
    }
    UComponent* Editor_ComponentRegistry::Create(const std::string& typeName)
    {
        auto& map = Components();
        auto it = map.find(typeName);
        
        if (it == map.end())
            return nullptr;

        return it->second();
    }
    const std::vector<std::string>& Editor_ComponentRegistry::AllComponents()
    {
        return ComponentsVector();
    }
}