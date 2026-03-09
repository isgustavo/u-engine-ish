#pragma once
#include "UComponent.h"
#include "UAsset.h"
#include <istream>

namespace uei
{
	enum EGrid
	{
		GRID_4,
		GRID_8
	};

	inline std::string GridToString(EGrid grid)
	{
		switch (grid)
		{
		case EGrid::GRID_4:
			return "4-Side-GRID";
		case EGrid::GRID_8:
			return "8-Side-GRID";
		}
	}

	inline EGrid StringToGrid(std::string grid)
	{
		if (grid == "4-Side-GRID")
			return EGrid::GRID_4;
		if (grid == "8-Side-GRID")
			return EGrid::GRID_8;
		return EGrid::GRID_4;
	}

	class CMovement : public UComponent
	{
	public:
		CMovement();
		~CMovement();

		UComponent* Clone() const override
		{
			auto* m = new CMovement();
			m->movementIndex = movementIndex;
			return m;
		}

		EGrid GridMovement() { return gridMovement; }
		EMovement GetMovement() { return currentMovement; }
		void SetMovement(EMovement movement) { currentMovement = movement; }
		
		inline std::string ComponentName() const override { return "CMovement"; }
		int GetMovementIndex() { return movementIndex; }

		std::vector<std::string>& AllMovementTypes()
		{
			if (allMovementTypesName.size() == 0)
			{
				allMovementTypesName.push_back(GridToString(EGrid::GRID_4));
				allMovementTypesName.push_back(GridToString(EGrid::GRID_8));
			}
			return allMovementTypesName;
		}

		void LoadComponent(UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		int movementIndex;
		std::vector<std::string> allMovementTypesName;

		EGrid gridMovement;
		EMovement currentMovement;

		void OnComponentAdd(class UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}
