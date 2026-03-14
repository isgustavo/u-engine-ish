#include "CStaticDraw.h"
#include "UEngine.h"

namespace uei
{
	CStaticDraw::CStaticDraw()
	{
	}

	CStaticDraw::~CStaticDraw()
	{
	}

	void uei::CStaticDraw::LoadComponent(std::istream& in)
	{

	}

	std::string CStaticDraw::SaveComponent() const
	{
		return std::string();
	}

	void CStaticDraw::OnShowEditor(UEngine& engine)
	{
		//ImGui::Text("Batch:");
		//ImGui::SameLine();
		//char editorNameBuffer[128];
		//strcpy_s(editorNameBuffer, sizeof(editorNameBuffer), batchName.c_str());
		////std::strcpy(editorNameBuffer, batchName.c_str());
		//if (ImGui::InputText("##BatchName", editorNameBuffer, IM_ARRAYSIZE(editorNameBuffer)))
		//{
		//	batchName = std::string(editorNameBuffer);
		//}
	}
}