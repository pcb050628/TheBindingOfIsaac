#pragma once
#include "ComponentGUI.h"

class GameObject;
class TransformGUI :
    public ComponentGUI
{
public:
    void RenderUpdate() override;

public:
    TransformGUI();
    ~TransformGUI();
};

