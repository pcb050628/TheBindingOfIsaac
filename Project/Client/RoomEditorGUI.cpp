#include "pch.h"
#include "RoomEditorGUI.h"

#include <Engine\ResourceManager.h>
#include <Engine\Room.h>

#include "ImGuiManager.h"
#include "ListGUI.h"
#include "RoomEditorRenderGUI.h"

RoomEditorGUI::RoomEditorGUI() : GUI("RoomEditor", "##RoomEditorGUI")
	, m_EditRoom(nullptr)
	, m_RenderGUI(nullptr)
	, m_SelectLayer(LAYER_TYPE::Object)
{
}

RoomEditorGUI::~RoomEditorGUI()
{
	if (m_EditRoom)
		delete m_EditRoom;
}

void RoomEditorGUI::RenderUpdate()
{
	bool active = IsActive();
	if (active && ImGui::BeginTabItem("RoomEditor##RoomEditorGUITabBar", &active))
	{
		if (!m_RenderGUI || !m_RenderGUI->IsActive())
			ActivateRenderGUI();

		ImGui::Text("Object"); ImGui::SameLine(); 
		if (ImGui::Button("##ObjectSelectCheckBox", ImVec2(20, 20)))
		{
			ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

			std::vector<std::string> objlist;
			ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::GAMEOBJECT, objlist);

			list->AddString(objlist);
			list->SetDelegate(this, (Delegate_1)&RoomEditorGUI::SelectObject);
			list->Activate();
		}
		ImGui::Spacing();

		std::string name = ToString(m_SelectObject);
		std::string label = "##RoomEditorGUISelectObjectInputText";
		ImGui::Text("SelectObject"); ImGui::SameLine(); ImGui::InputText(label.c_str(), (char*)name.data(), name.size(), ImGuiInputTextFlags_ReadOnly);

		ImGui::EndTabItem();
	}

	active ? Activate() : Deactivate();
}

void RoomEditorGUI::ActivateRenderGUI()
{
	if (!m_EditRoom)
		m_EditRoom = new Room;
	if (!m_RenderGUI)
		m_RenderGUI = (RoomEditorRenderGUI*)ImGuiManager::GetInst()->FindGUI("##RoomEditorRenderGUI");
	if (!m_RenderGUI->IsActive())
		m_RenderGUI->Activate();
	if (!m_RenderGUI->GetTarget())
		m_RenderGUI->SetTarget(this);
}

void RoomEditorGUI::SelectObject(DWORD_PTR _str)
{
	std::string strname = (char*)_str;
	m_SelectObject = ToWstring(strname);
}

void RoomEditorGUI::Activate()
{
	GUI::Activate();

	if (!m_EditRoom)
		m_EditRoom = new Room;
}
