#pragma once
#include "ComponentGUI.h"
class MeshRendererGUI :
    public ComponentGUI
{
public:
    void RenderUpdate() override;

public:
    MeshRendererGUI();
    ~MeshRendererGUI();
};

