#pragma once
#include "Script.h"

class SpotLight2DMove :
    public Script
{
public:
    void Update() override;

public:
    SpotLight2DMove();
    ~SpotLight2DMove();
};
