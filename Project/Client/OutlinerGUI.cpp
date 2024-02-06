#include "pch.h"
#include "OutlinerGUI.h"

#include "ImGuiManager.h"
#include "InspectorGUI.h"

#include <Engine/ChapterManager.h>
#include <Engine/Chapter.h>
#include <Engine/Room.h>
#include <Engine/GameObject.h>

OutlinerGUI::OutlinerGUI() : GUI("Outliner", "##OutlinerGUI")
	, m_SelectObject(nullptr)
	, m_ObjList{}
{
}

OutlinerGUI::~OutlinerGUI()
{
}

void OutlinerGUI::RenderUpdate()
{
	ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetAllObject(m_ObjList);

	VectorTreeNode(m_ObjList);

	m_ObjList.clear();
}

void OutlinerGUI::VectorTreeNode(std::vector<GameObject*>& _vector)
{
	for (int i = 0; i < _vector.size(); i++)
	{
		char buf[100];
		itoa(i, buf, 10);
		std::string Label = ToString(_vector[i]->GetName());
		Label += "##";
		Label += ToString(_vector[i]->GetName()) + buf;
		if (ImGui::TreeNode(Label.c_str()))
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				SetSelectObj(_vector[i]);
			}

			std::vector<GameObject*> vecChild = _vector[i]->GetChild();
			VectorTreeNode(vecChild);

			ImGui::TreePop();
		}
		else
		{
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			{
				SetSelectObj(_vector[i]);
			}
		}
	}
}

void OutlinerGUI::SetSelectObj(GameObject* _obj)
{
	if (_obj != m_SelectObject)
	{
		m_SelectObject = _obj;
		InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
		inspector->SetTargetObject(m_SelectObject);
	}
}
