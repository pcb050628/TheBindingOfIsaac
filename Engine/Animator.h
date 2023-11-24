#pragma once
#include "Component.h"
#include "Anim.h"

class Animator :
    public Component
{
    typedef Component Super;
private:
    std::map<std::wstring, Anim*> m_Anims;


public:
    virtual void Update() override;
    virtual void LateUpdate() override;
    virtual void Render() override;


public:
    Animator();
    ~Animator() override;
};

