#pragma once
#include "UComponent.h"

namespace uei
{
	class CNavGridModifier : public UComponent
	{
	public:
		CNavGridModifier(int inRows, int inColumns, int inWeight) : UComponent(),
			rows(inRows), columns(inColumns), weight(inWeight)
		{
			
		}

		int Rows() { return rows; }
		int Columns() { return columns; }
		int Weight() { return weight; }

	private:
		int rows;
		int columns;
		int weight;
	};
}