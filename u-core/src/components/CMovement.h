#pragma once
#include "UComponent.h"
#include "CMovementAnimation.h"

#include <istream>

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

namespace uei
{
	class CMovement : public UComponent
	{
	public:
		CMovement();
		~CMovement();

		EGrid GridMovement() const { return gridMovement; }
		EMovement GetLastMovement() const { return lastMovement; }
		EMovement GetCurrentMovement() const { return currentMovement; }
		void SetCurrentMovement(EMovement movement) 
		{
			lastMovement = currentMovement;
			currentMovement = movement; 
		}

		std::vector<sf::Vector2i> GetValidGridMovement() const;

		UComponent* Clone() override
		{
			return new CMovement(gridMovement);
		}

		inline std::string ComponentName() const override { return "CMovement"; }
		
		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(UEngine& engine) override;
		int GetEditorSize() const override;

	private:
		CMovement(EGrid inGridMovement);

		EGrid gridMovement;
		EMovement lastMovement;
		EMovement currentMovement;
	};
}
