#include <string>
#include "UEntity.h"

namespace uei
{
	bool UEntity::IsActive() const
	{
		return bIsActive;
	}
	const std::string& UEntity::GetTag() const
	{
		return tag;
	}
}