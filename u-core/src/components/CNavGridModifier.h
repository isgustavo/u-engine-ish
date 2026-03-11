#pragma once
#include "UComponent.h"

namespace uei
{
	class CNavGridModifier : public UComponent
	{
	public:
		CNavGridModifier(int inStartColumn = 0, int inColumns = 1, int inStartRow = 0, int inRows = 1, int inWeight = 100);

		int Columns() { return columns; }
		int StartColumn() { return startColumn; }

		int Rows() { return rows; }
		int StartRow() { return startRow; }

		int Weight() { return weight; }

		UComponent* Clone() const override
		{
			return new CNavGridModifier(startColumn, columns, startRow, rows, weight);
		}

		inline std::string ComponentName() const override { return "CNavGridModifier"; }

		void LoadComponent(class UEngine& engine, std::istream& in) override;
		std::string Save() const override;

	protected:
		void OnShowEditor(class UEngine& engine) override;
		int GetEditorSize(UEngine& engine) const override;

	private:
		int startColumn;
		int columns;
		int startRow;
		int rows;
		int weight;

		void OnComponentAdd(UEntity& entity) override;
		void OnComponentRemove(class UEntity& entity) override;
	};
}