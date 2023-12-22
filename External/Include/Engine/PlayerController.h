#pragma once
#include "Script.h"
class PlayerController :
    public Script
{
private:
    float m_fSpeed;

public:
    virtual void Update() override;

public:
    PlayerController();
    ~PlayerController();
};

