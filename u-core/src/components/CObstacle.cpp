#include "CObstacle.h"
#include "CNavGridModifier.h"
#include "entities/UEntity.h"

namespace uei
{
	CObstacle::CObstacle() : UComponent()
	{

	}

	CObstacle::~CObstacle()
	{

	}

	void CObstacle::OnComponentAdd(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CNavGridModifier>(true);
	}

	void CObstacle::OnComponentRemove(UEntity& entity)
	{
		entity.SetRequiredByOtherComponent<CNavGridModifier>(true);
	}

	void CObstacle::OnShowEditor(UEngine& engine)
	{

	}
}