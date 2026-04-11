#include "CNavGridModifier.h"

#include <imgui.h>
#include <iostream>
#include <sstream>

namespace uei
{
	CNavGridModifier::CNavGridModifier() : UComponent(), startColumn(0), columns(1), startRow(0), rows(1), weight(0)
	{

	}

	CNavGridModifier::CNavGridModifier(int inStartColumn, int inColumns, int inStartRow, int inRows, int inWeight) : UComponent(),
		startColumn(inStartColumn), columns(inColumns), startRow(inStartRow), rows(inRows), weight(inWeight)
	{

	}

	CNavGridModifier::~CNavGridModifier()
	{

	}

	void uei::CNavGridModifier::LoadComponent(std::istream & in)
	{
		Deserialize(in, startColumn, columns, startRow, rows, weight);
		//in >> startColumn >> columns >> startRow >> rows >> weight;
	}

	std::string CNavGridModifier::SaveComponent() const
	{
		return Serialize(startColumn, columns, startRow, rows, weight);
		/*std::stringstream ss;
		ss << startColumn << " " << columns << " " << startRow << " " << rows << " " << weight;
		return ss.str();*/
	}

	void CNavGridModifier::OnShowEditor(UEngine& engine)
	{
		ImGui::Text("Start Column");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::InputInt("##StartColumn", &startColumn, 0);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Text("Columns");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::InputInt("##Column", &columns, 0);
		ImGui::PopItemWidth();

		ImGui::Text("Start Row");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::InputInt("##StartRow", &startRow, 0);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::Text("Rows");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::InputInt("##Rows", &rows, 0);
		ImGui::PopItemWidth();

		ImGui::Text("Weight");
		ImGui::SameLine();
		ImGui::PushItemWidth(60);
		ImGui::InputInt("##Weight", &weight, 0);
		ImGui::PopItemWidth();
	}

	int uei::CNavGridModifier::GetEditorSize() const
	{
		return 120;
	}
}
