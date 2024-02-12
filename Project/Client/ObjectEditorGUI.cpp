#include "pch.h"
#include "ObjectEditorGUI.h"

#include <Engine\ScriptFactory.h>
#include <Engine\components.h>

#include "ImGuiManager.h"
#include "ListGUI.h"

ObjectEditorGUI::ObjectEditorGUI() : GUI("ObjectEditor", "ObjectEditorGUI")
	, m_EditObject(nullptr)
{
}

ObjectEditorGUI::~ObjectEditorGUI()
{
	if (m_EditObject)
		delete m_EditObject;
}

void ObjectEditorGUI::RenderUpdate()
{
	if (!m_EditObject)
		m_EditObject = new GameObject;

	//������Ʈ �̸�
	std::string objName = ToString(m_EditObject->GetName());
	std::string label = "##ObjectEditorGUIObjectNameInputText";
	ImGui::Text("Name"); ImGui::SameLine(); ImGui::InputText(label.c_str(), (char*)objName.data(), objName.capacity(), ImGuiInputTextFlags_ReadOnly);
	m_EditObject->SetName(ToWstring(objName));
	
	//������Ʈ
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (nullptr == m_EditObject->GetComponent((COMPONENT_TYPE)i))
		{
			std::string label = "Add";
			label += COMPONENT_TYPE_STRING[i].c_str();
			if (ImGui::Button(label.c_str()))
			{
				AddComponent((COMPONENT_TYPE)i);
			}
		}
		else
		{
			ImGui::Text(COMPONENT_TYPE_STRING[i].c_str());
			ImGui::SameLine();
			std::string label = "DeleteComponent##";
			label += COMPONENT_TYPE_STRING[i].c_str();
			if (ImGui::Button(label.c_str()))
			{
				m_EditObject->DeleteComponent((COMPONENT_TYPE)i);
			}
		}
	}
	
	//��ũ��Ʈ
	ImGui::SeparatorText("Script##ObjectEditorScriptSeparator");

	ImGui::Text("AddScript");
	ImGui::SameLine();
	if (ImGui::Button("##ObjectEditorScriptAddButton", ImVec2(20, 20)))
	{
		//����Ʈ ����

		ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

		std::vector<std::string> scriptlist;
		ScriptFactory::GetInst()->GetAllScriptName(scriptlist);

		list->AddString(scriptlist);
		list->SetDelegate(this, (Delegate_1)&ObjectEditorGUI::AddScript);
		list->Activate();
	}

	std::vector<std::string> list;
	m_EditObject->GetScriptName(list);
	for (std::string str : list)
	{
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		std::string label = "DeleteScript##";
		label += str.c_str();
		if (ImGui::Button(label.c_str()))
		{
			m_EditObject->DeleteScript(ToWstring(label));
		}
	}
}

void ObjectEditorGUI::AddScript(DWORD_PTR _str)
{
	string str((char*)_str);

	Script* scrpt = ScriptFactory::GetInst()->Find(ToWstring(str));
	if (nullptr == scrpt)
		return;

	m_EditObject->AddComponent(scrpt);
}

void ObjectEditorGUI::AddComponent(COMPONENT_TYPE _type)
{
	if (!m_EditObject)
		return;

	Component* comp = nullptr;

	switch (_type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		comp = new Transform;
		m_EditObject->AddComponent(comp);
		return;
		break;
	case COMPONENT_TYPE::COLLIDER2D:
		comp = new Collider2D;
		m_EditObject->AddComponent(comp);
		return;
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
		comp = new Animator2D;
		m_EditObject->AddComponent(comp);
		return;
		break;
	case COMPONENT_TYPE::LIGHT2D:
		comp = new Light2D;
		m_EditObject->AddComponent(comp);
		return;
		break;
	case COMPONENT_TYPE::CAMERA:
		comp = new Camera;
		m_EditObject->AddComponent(comp);
		return;
		break;
	case COMPONENT_TYPE::MESHRENDERER:
		comp = new MeshRenderer;
		m_EditObject->AddComponent(comp);
		return;
		break;
	//case COMPONENT_TYPE::COLLIDER3D:
	//	break;
	//case COMPONENT_TYPE::ANIMATOR3D:
	//	break;
	//case COMPONENT_TYPE::LIGHT3D:
	//	break;
	//case COMPONENT_TYPE::TILEMAP:
	//	break;
	//case COMPONENT_TYPE::PARTICLESYSTEM:
	//	break;
	//case COMPONENT_TYPE::SKYBOX:
	//	break;
	//case COMPONENT_TYPE::DECAL:
	//	break;
	//case COMPONENT_TYPE::LANDSCAPE:
	//	break;
	}
}
