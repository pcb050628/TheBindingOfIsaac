#pragma once
#include "GUI.h"

class GameObject;
class TransformGUI :
    public GUI
{
private:
    GameObject* m_TargetObject;
    
public:
    void RenderUpdate() override;

    void SetTargetObject(GameObject* _object);

public:
    TransformGUI();
    ~TransformGUI();
};

