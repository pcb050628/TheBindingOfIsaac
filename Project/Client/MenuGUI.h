#pragma once
#include "GUI.h"
class MenuGUI :
    public GUI
{
public:
    virtual void Render() override;
    virtual void RenderUpdate() override;

public:
    MenuGUI();
    ~MenuGUI();
};

