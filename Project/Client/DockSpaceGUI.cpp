#include "pch.h"
#include "DockSpaceGUI.h"

#include "ImGuiManager.h"

DockSpaceGUI::DockSpaceGUI(const std::string& _name, ImVec2 _dockSize) : GUI(_name, "##DockSpaceGUI")
	, m_DockSize(_dockSize)
{
	ImGuiManager::GetInst()->AddGUI(this);
}

DockSpaceGUI::~DockSpaceGUI()
{
}

void DockSpaceGUI::RenderUpdate()
{
	ImGui::SetWindowSize(m_DockSize);
	std::string id = GetName() + GetID();
	ImGui::DockSpace(ImGui::GetID(id.c_str()));
}
