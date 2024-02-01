#pragma once
#include "GUI.h"

class RoomEditorGUI;
class RoomEditorRenderGUI :
    public GUI
{
private:
    RoomEditorGUI*  m_Target;

    bool            m_bLeft;

public:
    RoomEditorGUI* GetTarget() { return m_Target; }
    void SetTarget(RoomEditorGUI* _target) { m_Target = _target; }

public:
    void RenderUpdate() override;

public:
    RoomEditorRenderGUI();
    ~RoomEditorRenderGUI();
};

