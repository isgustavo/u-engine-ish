#include "UComponent.h"
#include <istream>
#include <imgui.h>

namespace uei
{
    UComponent::UComponent() 
        : bCanRemove(true), bIsRequiredByOtherComponent(false), requiredByOtherComponent(0)
    { 
    }

    void UComponent::SetRequiredByOtherComponent(bool value)
    {
        if (value)
        {
            requiredByOtherComponent++;
        }
        else 
        {
            requiredByOtherComponent--;
            if (requiredByOtherComponent < 0)
                requiredByOtherComponent = 0;
        }

        bIsRequiredByOtherComponent = requiredByOtherComponent > 0;
    }

    void UComponent::ShowEditor(class UEngine& engine, std::function<void()> onRemove)
    {
        std::string name = ComponentName();
        std::string id = "##" + name;
        ImGui::BeginChild(
            id.c_str(),
            ImVec2(0, GetEditorSize(engine)),
            true
        );

        ImGui::Text(name.c_str());
        if (!bIsRequiredByOtherComponent)
        {
            if (bCanRemove)
            {
                ImGui::SameLine();
                if (ImGui::Button("Remove"))
                {
                    onRemove();
                }
            }
        }

        OnShowEditor(engine);

        ImGui::EndChild();
        ImGui::Spacing();
    }
}
