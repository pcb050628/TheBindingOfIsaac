#pragma once
#include "GUI.h"

#include <Engine\GameObject.h>

class ObjectEditorGUI :
    public GUI
{
private:
    GameObject* m_EditObject;

public:
    void RenderUpdate() override;

private:
    void AddComponent(COMPONENT_TYPE _type);

public:
    ObjectEditorGUI();
    ~ObjectEditorGUI();
};

