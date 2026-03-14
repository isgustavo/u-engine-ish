#include "CMovement.h"
#include "CMovementAnimation.h"

#include <imgui.h>
#include <sstream>
#include <iostream>


namespace uei
{
	CMovement::CMovement() : UComponent()
	{
		allMovementTypesName.push_back(GridToString(EGrid::GRID_4));
		allMovementTypesName.push_back(GridToString(EGrid::GRID_8));

		currentMovement = EMovement::NONE;
	}

	CMovement::CMovement(EGrid inGridMovement) : CMovement()
	{
		gridMovement = inGridMovement;
	}

	CMovement::~CMovement()
	{

	}

	void CMovement::OnShowEditor(UEngine& engine)
	{
		std::string gridMovementName = GridToString(gridMovement);
		ImGui::PushItemWidth(260);
		if (ImGui::BeginCombo("##MovementCombo", gridMovementName.c_str()))
		{
			for (int i = 0; i < allMovementTypesName.size(); ++i)
			{
				if (ImGui::Selectable(allMovementTypesName[i].c_str()))
				{
					gridMovement = StringToGrid(allMovementTypesName[i]);
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
	}

	int uei::CMovement::GetEditorSize() const
	{
		return 66;
	}
	
	void uei::CMovement::LoadComponent(std::istream& in)
	{
		std::string a0;
		Deserialize(in, a0);
		gridMovement = StringToGrid(a0);
	}
	
	std::string CMovement::SaveComponent() const
	{
		return Serialize(GridToString(gridMovement));
	}
}
