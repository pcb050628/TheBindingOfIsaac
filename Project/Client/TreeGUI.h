#pragma once
#include "GUI.h"

#include "ImGuiManager.h"

class TreeNode
{
private:
    TreeGUI*                m_Owner;
    std::string             m_Name;
    std::string             m_ID;

    TreeNode*               m_ParentNode;
    std::vector<TreeNode*>  m_ChildNode;

    DWORD_PTR               m_Data;

    bool                    m_bIsSelected;

public:
    DWORD_PTR GetData() { return m_Data; }

public:
    void RenderUpdate();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeGUI;
};

class TreeGUI :
    public GUI
{
    static UINT m_ID;
private:
    TreeNode*   m_Root;
    TreeNode*   m_Selected;

    bool        m_bShowRoot;
    bool        m_bSelect;
    bool        m_bMouseClickButton;

    GUI*        m_Inst;
    Delegate_1  m_Func;

public:
    TreeNode* AddNode(TreeNode* _parent, const std::string& _name, DWORD_PTR _data = NULL);
    void SetDelegate(GUI* _inst, Delegate_1 _func) { m_Inst = _inst; m_Func = _func; }

    void ClearNode() { if (m_Root) delete m_Root; m_Root = nullptr; }

    //0 : left , 1 : right
    void SetClickButton(bool _value) { m_bMouseClickButton = _value; }

private:
    void SelectCall(TreeNode* _node);

public:
    virtual void RenderUpdate() override;

public:
    TreeGUI(const std::string& _ID);
    ~TreeGUI();

    friend TreeNode;
};

