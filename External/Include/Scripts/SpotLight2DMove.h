#pragma once
#include <Engine\Script.h>

class SpotLight2DMove :
    public Script
{
public:
    void Update() override;

    void SaveToFile(FILE* _file) override;
    void LoadFromFile(FILE* _file) override;

    CLONE(SpotLight2DMove)
public:
    SpotLight2DMove();
    ~SpotLight2DMove();
};