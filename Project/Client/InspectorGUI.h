#pragma once
#include "GUI.h"

class GameObject;
class Resouce;

class ComponentGUI;

class InspectorGUI :
    public GUI
{
private:
    GameObject*         m_TargetObject;
    Resouce*            m_TargetResource;

    ComponentGUI*       m_ComGUI[(UINT)COMPONENT_TYPE::END];

public:
    void RenderUpdate() override;

    void SetTargetObject(GameObject* _object);
    void SetTargetResource(Resouce* _resouce);

    GameObject* GetTargetObject() { return m_TargetObject; }

    GUI* GetComponentGUI(COMPONENT_TYPE _type) { return m_ComGUI[(UINT)_type]; }

public:
    InspectorGUI();
    ~InspectorGUI();
};

