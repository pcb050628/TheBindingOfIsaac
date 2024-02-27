#pragma once
#include <Engine\Script.h>

class SpotLight2DMove :
    public Script
{
    SCRIPT(SpotLight2DMove)
public:
    void Update() override;

    CLONE(SpotLight2DMove)
public:
    SpotLight2DMove();
    ~SpotLight2DMove();
};
REGISTER_SCRIPT(SpotLight2DMove);