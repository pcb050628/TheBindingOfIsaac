#include "pch.h"
#include "ParamGUI.h"

#include "imgui.h"

bool ParamGUI::Param_INT(UINT _id, int* _data, const std::string& _desc)
{
    ImGui::Text(_desc.c_str());
    ImGui::SameLine();

    char szID[256] = {};
    sprintf_s(szID, "##int%d", _id);
    if (ImGui::InputInt(szID, _data))
    {
        return true;
    }

    return false;
}
