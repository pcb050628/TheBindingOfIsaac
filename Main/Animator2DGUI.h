#pragma once
#include "ComponentGUI.h"

class Texture;
class Animator2DGUI :
    public ComponentGUI
{
private:
    bool        m_bCreateMod;
    Texture*    m_CreateAnimTex;

public:
    virtual void RenderUpdate() override;

    void SetCreateAnimTex(Texture* _tex) { m_CreateAnimTex = _tex; }

public:
    Animator2DGUI();
    ~Animator2DGUI();
};

