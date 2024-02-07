#pragma once
#include "Script.h"

class SpotLight2DMove :
    public Script
{
    SCRIPT(SpotLight2DMove)
public:
    void Update() override;

public:
    SpotLight2DMove();
    ~SpotLight2DMove();
};
REGISTER_SCRIPT(SpotLight2DMove);