#pragma once
#include "UComponent.h"

namespace uei
{
	class CNavGridModifier : public UComponent
	{
	public:
		CNavGridModifier();
		~CNavGridModifier();

		int GetColumns() const { return columns; }
		int GetStartColumn() const { return startColumn; }

		int GetRows() const { return rows; }
		int GetStartRow() const { return startRow; }

		int GetWeight() const { return weight; }

		UComponent* Clone() override
		{
			return new CNavGridModifier(startColumn, columns, startRow, rows, weight);
		}

		inline std::string ComponentName() const override { return "CNavGridModifier"; }

		void LoadComponent(std::istream & in) override;
		std::string SaveComponent() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize() const override;

	private:
		CNavGridModifier(int inStartColumn, int inColumns, int inStartRow, int inRows, int inWeight);

		int startColumn;
		int columns;
		int startRow;
		int rows;
		int weight;
	};
}