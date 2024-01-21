#pragma once
#include "ComponentGUI.h"

enum class PROJ_TYPE;
enum class LAYER_TYPE;
class CameraGUI :
    public ComponentGUI
{
private:
    std::vector<std::string>    m_ProjString;
    int                         m_SelectInt;

public:
    void RenderUpdate() override;

private:
    std::string GetLayerName(LAYER_TYPE _layer);

public:
    CameraGUI();
    ~CameraGUI();
};

