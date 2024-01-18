#pragma once
#include "ComponentGUI.h"

class Texture;
class Anim;
class Animator2DGUI :
    public ComponentGUI
{
private:
    std::wstring    m_ExistingAnim;
    Anim*           m_AdditionalAnim;

    bool            m_bCreateMod;
    int             m_iNameInitSize;

public:
    virtual void RenderUpdate() override;

    void SetCreateAnimTex(Texture* _tex);

private:
    void CreateAdditionalAnim();
    void DeleteAdditionalAnim();

public:
    Animator2DGUI();
    ~Animator2DGUI();
};

