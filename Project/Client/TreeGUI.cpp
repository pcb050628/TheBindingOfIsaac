#include "pch.h"
#include "TreeGUI.h"

TreeNode::TreeNode()
	: m_Owner(nullptr)
	, m_ParentNode(nullptr)
	, m_Data(NULL)
	, m_bIsSelected(false)
{
}

TreeNode::~TreeNode()
{
	for (TreeNode* node : m_ChildNode)
	{
		delete node;
	}
	m_ChildNode.clear();
}

void TreeNode::RenderUpdate()
{
	std::string label = m_Name + m_ID;

	UINT flag = 0;
	if (m_bIsSelected)
		flag |= ImGuiTreeNodeFlags_Selected;
	if (m_ChildNode.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(label.c_str(), flag))
	{
		UINT MouseButton = ImGuiMouseButton_Left;
		if (m_Owner->m_bMouseClickButton)
			MouseButton = ImGuiMouseButton_Right;

		if (ImGui::IsItemClicked(MouseButton))
		{
			m_Owner->SelectCall(this);
		}

		for (TreeNode* node : m_ChildNode)
		{
			node->RenderUpdate();
		}

		ImGui::TreePop();
	}
	else
	{
		UINT MouseButton = ImGuiMouseButton_Left;
		if (m_Owner->m_bMouseClickButton)
			MouseButton = ImGuiMouseButton_Right;

		if (ImGui::IsItemClicked(MouseButton))
		{
			m_Owner->SelectCall(this);
		}
	}
}

TreeGUI::TreeGUI(const std::string& _ID) : GUI("TreeGUI", "##" + _ID)
	, m_Root(nullptr)
	, m_bShowRoot(false)
	, m_Selected(nullptr)
	, m_bMouseClickButton(true)
{}

UINT TreeGUI::m_ID = 0;

TreeGUI::~TreeGUI()
{
	if (m_Root)
		delete m_Root;
}

void TreeGUI::RenderUpdate()
{
	if (m_bShowRoot)
	{
		m_Root->RenderUpdate();
	}
	else
	{
		for (TreeNode* node : m_Root->m_ChildNode)
		{
			node->RenderUpdate();
		}
	}
}


TreeNode* TreeGUI::AddNode(TreeNode* _parent, const std::string& _name, DWORD_PTR _data)
{
	TreeNode* node = new TreeNode;

	node->m_Owner = this;

	//name
	node->m_Name = _name;

	//id
	char buff[50] = {};
	sprintf_s(buff, "##%d", m_ID++);
	node->m_ID = buff;

	//data
	node->m_Data = _data;

	if (!_parent && !m_Root)
	{
		m_Root = node;
	}
	else
	{
		if (!_parent)
			_parent = m_Root;

		_parent->m_ChildNode.push_back(node);
		node->m_ParentNode = _parent;
	}

	return node;
}

void TreeGUI::SelectCall(TreeNode* _node)
{
	if (m_Selected)
		m_Selected->m_bIsSelected = false;

	assert(_node);

	m_Selected = _node;
	m_Selected->m_bIsSelected = true;
	m_bSelect = true;

	if (m_Inst && m_Func)
	{
		(m_Inst->*m_Func)((DWORD_PTR)m_Selected);
	}
}
