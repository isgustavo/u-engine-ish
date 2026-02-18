#include <string>
#include "UEntity.h"

namespace uei
{
	UEntity::~UEntity()
	{
		/*for (auto& [key, value] : components)
		{
			if (value == nullptr) continue;
			delete value;
		}*/
		//components.clear();
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
	void UEntity::SetActive(bool value)
	{
		bIsActive = value;
	}
	std::unordered_map<std::type_index, uei::UComponent*>& UEntity::Components()
	{
		return components;
	}
}