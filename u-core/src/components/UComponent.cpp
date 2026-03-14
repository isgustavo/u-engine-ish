#include "UComponent.h"

#include <istream>
#include <imgui.h>

namespace uei
{
    UComponent::UComponent(bool canRemove)
        : bCanRemove(canRemove), requiredByOtherComponent(0)
    {

    }

    void UComponent::Start(UEngine& engine)
    {

    }

    bool UComponent::IsRequiredByOtherComponent() const
    { 
        return requiredByOtherComponent > 0; 
    }

    void UComponent::SetRequiredByOtherComponent(bool value)
    {
        if (value)
        {
            requiredByOtherComponent++;
        }
        else 
        {
            if(requiredByOtherComponent > 1)
                requiredByOtherComponent--;
        }
    }

    void UComponent::ShowEditor(class UEngine& engine, std::function<void()> onRemove)
    {
        std::string name = ComponentName();
        std::string id = "##" + name;
        ImGui::BeginChild(
            id.c_str(),
            ImVec2(0, GetEditorSize()),
            true
        );

        ImGui::Text(name.c_str());
        if (!IsRequiredByOtherComponent())
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
