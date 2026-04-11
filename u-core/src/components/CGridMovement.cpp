#include "CGridMovement.h"
#include "CMovementAnimation.h"
#include "CNavGridModifier.h"
#include "entities/UEntity.h"

#include <imgui.h>
#include <sstream>
#include <iostream>

namespace uei
{
	CGridMovement::CGridMovement() : UComponent()
	{
		lastMovement = EMovement::UP;
		currentMovement = EMovement::NONE;
		gridMovement = EGrid::GRID_4;
	}

	CGridMovement::CGridMovement(EGrid inGridMovement) : CGridMovement()
	{
		gridMovement = inGridMovement;
	}

	CGridMovement::~CGridMovement()
	{

	}

	void CGridMovement::SetNextMovement(sf::Vector2i& startPosition, sf::Vector2i& nextPosition, EMovement movement)
	{
		lastMovement = currentMovement;
		currentMovement = movement;
		gridMovementLerpTime = 0.0f;
		gridStartMovement = startPosition;
		gridTargetMovement = nextPosition;
		SetStop(false);
	}

	std::vector<sf::Vector2i> CGridMovement::GetValidGridMovement() const
	{
		std::vector<sf::Vector2i> validGridMovement;
		validGridMovement.push_back(MovementToVector(EMovement::UP));
		validGridMovement.push_back(MovementToVector(EMovement::LEFT));
		validGridMovement.push_back(MovementToVector(EMovement::DOWN));
		validGridMovement.push_back(MovementToVector(EMovement::RIGHT));

		if (gridMovement == GRID_8)
		{
			validGridMovement.push_back(MovementToVector(EMovement::UP_LEFT));
			validGridMovement.push_back(MovementToVector(EMovement::UP_RIGHT));
			validGridMovement.push_back(MovementToVector(EMovement::DOWN_LEFT));
			validGridMovement.push_back(MovementToVector(EMovement::DOWN_RIGHT));
		}

		return validGridMovement;
	}

	void CGridMovement::OnShowEditor(UEngine& engine)
	{
		std::string gridMovementName = GridToString(gridMovement);
		ImGui::PushItemWidth(260);
		if (ImGui::BeginCombo("##MovementCombo", gridMovementName.c_str()))
		{
			std::vector<std::string> allMovementTypesName;
			allMovementTypesName.push_back(GridToString(EGrid::GRID_4));
			allMovementTypesName.push_back(GridToString(EGrid::GRID_8));

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

	int uei::CGridMovement::GetEditorSize() const
	{
		return 70;
	}
	
	void uei::CGridMovement::LoadComponent(std::istream& in)
	{
		std::string a0;
		Deserialize(in, a0);
		gridMovement = StringToGrid(a0);
	}
	
	std::string CGridMovement::SaveComponent() const
	{
		return Serialize(GridToString(gridMovement));
	}
}
