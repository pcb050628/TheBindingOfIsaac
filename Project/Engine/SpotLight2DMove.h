#pragma once
#include "Script.h"

class SpotLight2DMove :
    public Script
{
public:
    void Update() override;
    Script* Instance() override;

public:
    SpotLight2DMove();
    ~SpotLight2DMove();
};

