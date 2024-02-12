#pragma once
#include "GUI.h"

#include <Engine\GameObject.h>

class ObjectEditorGUI :
    public GUI
{
private:
    GameObject* m_EditObject;

public:
    void AddScript(DWORD_PTR _str);

private:
    void AddComponent(COMPONENT_TYPE _type);

public:
    void RenderUpdate() override;

public:
    ObjectEditorGUI();
    ~ObjectEditorGUI();
};

