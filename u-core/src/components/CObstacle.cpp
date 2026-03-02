#include "CObstacle.h"
namespace uei
{
	uei::CObstacle::CObstacle()
	{
	}
	CObstacle::~CObstacle()
	{
	}
	void CObstacle::LoadComponent(UEngine& engine, std::istream& in)
	{
	}
	std::string CObstacle::Save() const
	{
		return std::string();
	}
	void CObstacle::OnShowEditor(UEngine& engine)
	{
	}
	int CObstacle::GetEditorSize(UEngine& engine) const
	{
		return 33;
	}
	void CObstacle::OnComponentAdd(UEntity& entity)
	{
	}
	void CObstacle::OnComponentRemove(UEntity& entity)
	{
	}
}