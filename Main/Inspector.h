#pragma once
#include "GUI.h"

class GameObject;
class Resouce;

class ComponentGUI;

class Inspector :
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

public:
    Inspector();
    ~Inspector();
};

