#pragma once
#include "ComponentGUI.h"
class Light2DGUI :
    public ComponentGUI
{
private:
    std::vector<std::string> m_lightType;

public:
    void RenderUpdate() override;

public:
    Light2DGUI();
    ~Light2DGUI();
};

