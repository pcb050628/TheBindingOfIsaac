#pragma once
#include "ResourceGUI.h"

class TextureGUI :
    public ResourceGUI
{
public:
    void RenderUpdate() override;

public:
    TextureGUI();
    ~TextureGUI();
};

