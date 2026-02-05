#include <string>
#include "UEntity.h"

namespace uei
{
	UEntity::~UEntity()
	{
		for (const auto& [key, value] : components)
		{
			delete value;
		}
		components.clear();
	}
	std::string& UEntity::Name()
	{
		return name;
	}
	void UEntity::Name(std::string inName)
	{
		name = inName;
	}
	bool UEntity::IsActive() const
	{
		return bIsActive;
	}
	std::unordered_map<std::type_index, uei::UComponent*>& UEntity::Components()
	{
		return components;
	}
}