#pragma once
#include "ComponentGUI.h"

class Texture;
class Anim;
class Animator2DGUI :
    public ComponentGUI
{
private:
    int             m_iSelectedIdx;

public:
    virtual void RenderUpdate() override;

    virtual void AddAnim(DWORD_PTR _str);

public:
    Animator2DGUI();
    ~Animator2DGUI();
};

