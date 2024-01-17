#pragma once
#include "ComponentGUI.h"

class Animator2DGUI :
    public ComponentGUI
{
public:
    virtual void RenderUpdate() override;

public:
    Animator2DGUI();
    ~Animator2DGUI();
};

