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

    TreeNode*   m_DragNode;
    TreeNode*   m_DropNode;

    bool        m_bShowRoot;

    GUI*        m_SelectInst;
    Delegate_1  m_SelectFunc;
    bool        m_bSelect;

    GUI*        m_DragDropInst;
    Delegate_2  m_DragDropFunc;
    bool        m_bDragDrop;

public:
    TreeNode* AddNode(TreeNode* _parent, const std::string& _name, DWORD_PTR _data = NULL);

    //delegate info : 1 - selectnode (*TreeNode)
    void SetSelectDelegate(GUI* _inst, Delegate_1 _func) { m_SelectInst = _inst; m_SelectFunc = _func; }

    //delegate info : 1 - dropnode/ 2 - dragnode (*TreeNode)
    void SetDragDropDelegate(GUI* _inst, Delegate_2 _func) { m_DragDropInst = _inst; m_DragDropFunc = _func; }

    void ClearNode() { if (m_Root) delete m_Root; m_Root = nullptr; }



private:
    void SelectCall(TreeNode* _node);

    void SetDragNode(TreeNode* _node) { m_DragNode = _node; }
    void SetDropNode(TreeNode* _node) { m_DropNode = _node; m_bDragDrop = true; }

public:
    virtual void RenderUpdate() override;

public:
    TreeGUI(const std::string& _ID);
    ~TreeGUI();

    friend TreeNode;
};

