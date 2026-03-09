#include "CMovement.h"
#include <imgui.h>
#include <sstream>
#include <iostream>

namespace uei
{
	CMovement::CMovement() : UComponent(), movementIndex(0)
	{
		currentMovement = EMovement::NONE;
	}

	CMovement::~CMovement()
	{
	}

	void CMovement::OnShowEditor(UEngine& engine)
	{
		ImGui::PushItemWidth(260);
		if (ImGui::BeginCombo("##MovementCombo", AllMovementTypes()[movementIndex].c_str()))
		{
			for (int i = 0; i < AllMovementTypes().size(); ++i)
			{
				if (ImGui::Selectable(AllMovementTypes()[i].c_str()))
				{
					movementIndex = i;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}

	int CMovement::GetEditorSize(UEngine& engine) const
	{
		return 66;
	}
	
	void CMovement::OnComponentAdd(UEntity& entity)
	{
	}
	
	void CMovement::OnComponentRemove(UEntity& entity)
	{
	}
	
	void CMovement::LoadComponent(UEngine& engine, std::istream& in)
	{
		in >> movementIndex;
		gridMovement = StringToGrid(AllMovementTypes()[movementIndex]);
	}
	
	std::string CMovement::Save() const
	{
		std::ostringstream ss;
		ss << movementIndex;
		return ss.str();
	}
}
