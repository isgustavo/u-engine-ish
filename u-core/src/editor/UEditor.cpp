#include "UEditor.h"

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
    void UEditor::Register(const std::string& typeName, ComponentFunction cf)
    {
        auto& map = Components();
        map.emplace(typeName, cf);
        auto& vector = ComponentsVector();
        vector.push_back(typeName);
    }
    UComponent* UEditor::Create(const std::string& typeName)
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
}