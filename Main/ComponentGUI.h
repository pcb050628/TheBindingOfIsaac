#pragma once
#include "GUI.h"
class ComponentGUI :
    public GUI
{
private:
    GameObject*     m_TargetObject;
    COMPONENT_TYPE  m_Type;

public:
    virtual void RenderUpdate() override;
    void SetTargetObject(GameObject* _object);

    GameObject* GetTargetObject() { return m_TargetObject; }

public:
    ComponentGUI(const std::string& _strName, const std::string& _strID, COMPONENT_TYPE _type);
    virtual ~ComponentGUI();
};

