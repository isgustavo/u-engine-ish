#include "CNavGridModifier.h"
#include <imgui.h>
#include <iostream>
#include <sstream>

namespace uei
{
	CNavGridModifier::CNavGridModifier(int inStartColumn, int inColumns, int inStartRow, int inRows, int inWeight) : UComponent(),
		startColumn(inStartColumn), columns(inColumns), startRow(inStartRow), rows(inRows), weight(inWeight)
	{

	}

	void CNavGridModifier::LoadComponent(UEngine& engine, std::istream& in)
	{
		in >> startColumn >> columns >> startRow >> rows >> weight;
	}

	std::string CNavGridModifier::Save() const
	{
		std::stringstream ss;
		ss << startColumn << " " << columns << " " << startRow << " " << rows << " " << weight;
		return ss.str();
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

	int CNavGridModifier::GetEditorSize(UEngine& engine) const
	{
		return 120;
	}

	void CNavGridModifier::OnComponentAdd(UEntity& entity)
	{
	}

	void CNavGridModifier::OnComponentRemove(UEntity& entity)
	{
	}


}
