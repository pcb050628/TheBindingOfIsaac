#include "pch.h"
#include "RoomEditorGUI.h"

#include <Engine\Device.h>

#include <Engine\RenderManager.h>
#include <Engine\ResourceManager.h>
#include <Engine\Room.h>
#include <Engine\GameObject.h>

#include "ImGuiManager.h"
#include "ModalBoxGUI.h"
#include "DockSpaceGUI.h"
#include "ListGUI.h"
#include "RoomEditorRenderGUI.h"

RoomEditorGUI::RoomEditorGUI() : GUI("RoomEditor", "##RoomEditorGUI")
	, m_EditRoom(nullptr)
	, m_RenderGUI(nullptr)
	, m_SelectLayer(LAYER_TYPE::Object)
{
	//m_DockSpace = new DockSpaceGUI("RoomEditor Space", ImVec2(1800, 900));
	//m_RenderGUI = new RoomEditorRenderGUI;

	Deactivate();
}

RoomEditorGUI::~RoomEditorGUI()
{
	if (m_EditRoom)
		delete m_EditRoom;

	if (m_RenderGUI)
		delete m_RenderGUI;
}

void RoomEditorGUI::RenderUpdate()
{
	if (!m_RenderGUI || !m_RenderGUI->IsActive())
		ActivateRenderGUI();

	if (!m_EditRoom)
	{
		ImGui::Spacing();
		if (ImGui::Button("Create New Room"))
		{
			CreateNewRoom();
		}
	}
	else
	{
		std::string roomname = ToString(m_EditRoom->GetResourceName());
		std::string roomlabel = "##RoomEditorGUIRoomNameInputText";
		ImGui::Text("Room Name"); ImGui::SameLine(); ImGui::InputText(roomlabel.c_str(), (char*)roomname.data(), roomname.capacity());
		m_EditRoom->SetResourceName(ToWstring(roomname));


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

		ImGui::Text("Save"); ImGui::SameLine();
		if (ImGui::Button("##RoomEditorGUISaveButton", ImVec2(20, 20)))
		{
			if(m_EditRoom->GetResourceName() != L"")
				m_EditRoom->Save();
			else
			{
				ModalBoxGUI* modalBox = (ModalBoxGUI*)ImGuiManager::GetInst()->FindGUI("##ModalBoxGUI");
				modalBox->Set("Warning", "Set Room Name!");
				modalBox->Activate();
			}
		}
		ImGui::Spacing();


		m_EditRoom->Update();
		m_EditRoom->LateUpdate();

		RenderManager::GetInst()->EditRender(m_EditRoom->GetMainCam()->GetCamera());

		ImVec2 windowSize = ImGui::GetWindowSize();
		windowSize.y -= 150;

		ImGui::Image(Device::GetInst()->GetEditRenderTarget()->GetSRV().Get(), windowSize);
	}


	if (m_RenderGUI)
	{
		//m_RenderGUI->Render();
	}
}

void RoomEditorGUI::ActivateRenderGUI()
{
	if (!m_RenderGUI)
		m_RenderGUI = (RoomEditorRenderGUI*)ImGuiManager::GetInst()->FindGUI("##RoomEditorRenderGUI");
	if (!m_RenderGUI->IsActive())
		m_RenderGUI->Activate();
	if (!m_RenderGUI->GetTarget())
		m_RenderGUI->SetTarget(this);
}

void RoomEditorGUI::CreateNewRoom()
{
	if (m_EditRoom)
		delete m_EditRoom;
	m_EditRoom = new Room;
	m_EditRoom->SetResourceName(L"New_Room");

	GameObject* obj = new GameObject;
	obj->AddComponent(new Transform);
	obj->AddComponent(new Camera);

	m_EditRoom->AddObject(obj, LAYER_TYPE::Camera, false);

	obj = new GameObject;
	obj->Load(L"test_rock.gobj");

	m_EditRoom->AddObject(obj, LAYER_TYPE::Object, false);
}

void RoomEditorGUI::SelectObject(DWORD_PTR _str)
{
	std::string strname = (char*)_str;
	m_SelectObject = ToWstring(strname);
}

void RoomEditorGUI::Activate()
{
	GUI::Activate();

	//if (m_DockSpace) m_DockSpace->Activate();
	//if (m_RenderGUI) m_RenderGUI->Activate();
}

void RoomEditorGUI::Deactivate()
{
	GUI::Deactivate();

	//if (m_DockSpace) m_DockSpace->Deactivate();
	//if (m_RenderGUI) m_RenderGUI->Deactivate();
}
