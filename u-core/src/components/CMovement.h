#pragma once
#include "UComponent.h"
#include <istream>

namespace uei
{
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
		
		inline std::string ComponentName() const override { return "CMovement"; }
		int GetMovementIndex() { return movementIndex; }

		std::vector<std::string>& AllMovementTypes()
		{
			if (allMovementTypesName.size() == 0)
			{
				allMovementTypesName.push_back("4-Side-GRID");
				allMovementTypesName.push_back("8-Side-GRID");
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

		void OnComponentAdd(class UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}
