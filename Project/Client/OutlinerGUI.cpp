#include "pch.h"
#include "OutlinerGUI.h"

#include "ImGuiManager.h"
#include "InspectorGUI.h"

#include "TreeGUI.h"

#include <Engine/TaskManager.h>
#include <Engine/ChapterManager.h>
#include <Engine/Chapter.h>
#include <Engine/Room.h>
#include <Engine/GameObject.h>

OutlinerGUI::OutlinerGUI() : GUI("Outliner", "##OutlinerGUI")
	, m_ObjList{}
{
	m_TreeGUI = new TreeGUI("##OutlinerTreeGUI");
	AddChild(m_TreeGUI);


	m_TreeGUI->SetSelectDelegate(this, (Delegate_1)&OutlinerGUI::SetSelectObj);
}

OutlinerGUI::~OutlinerGUI()
{
}

void OutlinerGUI::RenderUpdate()
{
	if (TaskManager::GetInst()->IsDoSomething())
	{
		ResetTree();
		TaskManager::GetInst()->SetDoSomething(false);
	}
}

void OutlinerGUI::ResetTree()
{
	m_TreeGUI->ClearNode();

	ChapterManager::GetInst()->GetCurChapter()->GetCurRoom()->GetAllObject(m_ObjList);
	TreeNode* root = m_TreeGUI->AddNode(nullptr, "RootNode");
	SetObjectNode(root, m_ObjList);
}

void OutlinerGUI::SetObjectNode(TreeNode* _parent, std::vector<GameObject*>& m_objList)
{
	for (GameObject* obj : m_objList)
	{
		TreeNode* node = m_TreeGUI->AddNode(_parent, ToString(obj->GetName()), (DWORD_PTR)obj);
		if (!obj->GetChild().empty())
		{
			SetObjectNode(node, obj->GetChild());
		}
	}
}

void OutlinerGUI::SetSelectObj(DWORD_PTR _tree)
{
	if (((TreeNode*)_tree)->GetData() == NULL)
		return;

	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
	inspector->SetTargetObject((GameObject*)((TreeNode*)_tree)->GetData());
}
