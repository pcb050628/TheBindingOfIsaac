#pragma once
#include "Script.h"
class CameraController :
    public Script
{
private:
    float m_fSpeed;

public:
    virtual void Update() override;

public:
    CameraController();
    ~CameraController();
};

