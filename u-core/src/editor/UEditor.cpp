#include "UEditor.h"
#include "components/UComponent.h"
#include "systems/USystem.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <map>
#include <iostream>
#include <fstream>

namespace uei
{
    static std::unordered_map<std::string, UEditor::ComponentFunction>& Components()
    {
        static std::unordered_map<std::string, UEditor::ComponentFunction> components;
        return components;
    }
    static std::vector<std::string>& ComponentsVector()
    {
        static std::vector<std::string> components;
        return components;
    }
    void UEditor::RegisterComponent(const std::string& typeName, ComponentFunction cf)
    {
        auto& map = Components();
        map.emplace(typeName, cf);
        auto& vector = ComponentsVector();
        vector.push_back(typeName);
    }
    UComponent* UEditor::CreateComponent(const std::string& typeName)
    {
        auto& map = Components();
        auto it = map.find(typeName);

        if (it == map.end())
            return nullptr;

        return it->second();
    }
    const std::vector<std::string>& UEditor::AllComponents()
    {
        return ComponentsVector();
    }


    static std::unordered_map<std::string, UEditor::SystemFunction>& Systems()
    {
        static std::unordered_map<std::string, UEditor::SystemFunction> systems;
        return systems;
    }
    static std::vector<std::string>& SystemsVector()
    {
        static std::vector<std::string> systems;
        return systems;
    }
    void UEditor::RegisterSystem(const std::string& typeName, SystemFunction sf)
    {
        auto& map = Systems();
        map.emplace(typeName, sf);
        auto& vector = SystemsVector();
        vector.push_back(typeName);
    }
    USystem* UEditor::CreateSystem(const std::string& typeName)
    {
        auto& map = Systems();
        auto it = map.find(typeName);

        if (it == map.end())
            return nullptr;

        return it->second();
    }
    const std::vector<std::string>& UEditor::AllSystems()
    {
        return SystemsVector();
    }
}