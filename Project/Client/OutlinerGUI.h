#pragma once
#include "GUI.h"

class GameObject;
class TreeGUI;
class OutlinerGUI :
    public GUI
{
private:
    std::vector<GameObject*>    m_ObjList;
    TreeGUI*                    m_TreeGUI;

public:
    void RenderUpdate() override;

private:
    void ResetTree();
    void SetObjectNode(class TreeNode* _parent, std::vector<GameObject*>& m_objList);

public:
    void SetSelectObj(DWORD_PTR _tree);
    void AddObjectChild(DWORD_PTR _str);

public:
    OutlinerGUI();
    ~OutlinerGUI();
};

