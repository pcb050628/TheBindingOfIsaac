#pragma once
#include "GUI.h"

class GameObject;
class OutlinerGUI :
    public GUI
{
private:
    GameObject*                 m_SelectObject;
    std::vector<GameObject*>    m_ObjList;

public:
    void RenderUpdate() override;

    GameObject* GetSelectObj() { return m_SelectObject; }

private:
    void VectorTreeNode(std::vector<GameObject*>& _vector);
    void SetSelectObj(GameObject* _obj);

public:
    OutlinerGUI();
    ~OutlinerGUI();
};

