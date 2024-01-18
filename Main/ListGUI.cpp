#include "pch.h"
#include "ListGUI.h"

ListGUI::ListGUI() : GUI("", "##ListGUI")
	, m_strData{}
	, m_LatestClickData{}
	, m_CallBackFunc(nullptr)
{
    Deactivate();
    SetModal(true);
}

ListGUI::~ListGUI()
{
}

void ListGUI::RenderUpdate()
{
    ImVec2 vWinSize = ImGui::GetWindowContentRegionMax();
    vWinSize.y -= 27;

    static int item_current_idx = 0; // Here we store our selection data as an index.
    if (ImGui::BeginListBox("##ListBox", vWinSize))
    {
        for (int i = 0; i < m_strData.size(); i++)
        {
            const bool is_selected = (item_current_idx == i);

            if (ImGui::Selectable(m_strData[i].c_str(), is_selected))
                item_current_idx = i;

            // 리스트 중 해당 항목이 클릭되면 하이라이트 걸어줌
            if (is_selected)
                ImGui::SetItemDefaultFocus();

            // 리스트 항목 중 더블클릭이 발생한다면            
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_LatestClickData = m_strData[i];

                if (nullptr != m_CallBackFunc)
                    m_CallBackFunc((DWORD_PTR)m_LatestClickData.c_str());

                Deactivate();
            }
        }
        ImGui::EndListBox();
    }
}

void ListGUI::Deactivate()
{
	GUI::Deactivate();
	m_strData.clear();
}
