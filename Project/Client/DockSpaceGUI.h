#pragma once
#include "GUI.h"

class DockSpaceGUI :
    public GUI
{
private:
    ImVec2 m_DockSize;

public:
    virtual void RenderUpdate() override;

public:
    //don't make in imguiManager
    //don't add gui in imguimanager, dockspacegui do it self
    //don't delete, dockspacegui will delete by imguimanager
    DockSpaceGUI(const std::string& _name, ImVec2 _dockSize);
    ~DockSpaceGUI();
};

