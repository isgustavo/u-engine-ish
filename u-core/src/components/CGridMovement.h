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
	class CGridMovement : public UComponent
	{
	public:
		CGridMovement();
		~CGridMovement();

		float GetLerpTime() const { return gridMovementLerpTime; }
		void UpdateLerpTime(float value) { gridMovementLerpTime += value; }
		sf::Vector2i& GetGridStartMovement() { return gridStartMovement; }
		sf::Vector2i& GetGridTargetMovement() { return gridTargetMovement; }

		EGrid GridMovement() const { return gridMovement; }
		EMovement GetLastMovement() const { return lastMovement; }
		EMovement GetCurrentMovement() const { return currentMovement; }
		EMovement GetValidMovement() const { return currentMovement == EMovement::NONE ? lastMovement : currentMovement; }

		bool IsStop() { return stop; }
		void SetStop(bool v) { stop = v; }

		void SetNextMovement(sf::Vector2i& startPosition, sf::Vector2i& currentPosition, EMovement movement);

		std::vector<sf::Vector2i> GetValidGridMovement() const;

		UComponent* Clone() override
		{
			return new CGridMovement(gridMovement);
		}

		inline std::string ComponentName() const override { return "CGridMovement"; }
		
		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(UEngine& engine) override;
		int GetEditorSize() const override;

	private:
		CGridMovement(EGrid inGridMovement);

		bool stop;
		EGrid gridMovement;
		EMovement lastMovement;
		EMovement currentMovement;
		float gridMovementLerpTime = 2.0f;
		sf::Vector2i gridStartMovement;
		sf::Vector2i gridTargetMovement;
	};
}
