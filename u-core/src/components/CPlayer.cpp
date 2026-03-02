#include "CPlayer.h"
namespace uei
{
	CPlayer::CPlayer()
	{
	}
	CPlayer::~CPlayer()
	{
	}
	void CPlayer::LoadComponent(UEngine& engine, std::istream& in)
	{
	}
	std::string CPlayer::Save() const
	{
		return std::string();
	}
	void CPlayer::OnShowEditor(UEngine& engine)
	{
	}
	int CPlayer::GetEditorSize(UEngine& engine) const
	{
		return 33;
	}
	void CPlayer::OnComponentAdd(UEntity& entity)
	{
	}
	void CPlayer::OnComponentRemove(UEntity& entity)
	{
	}
}