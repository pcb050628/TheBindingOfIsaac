#pragma once
#include "GUI.h"

class AnimEditorGUI;
class RoomEditorGUI;
class EditorGUI :
    public GUI
{
private:
	AnimEditorGUI* m_AnimEditor;
	RoomEditorGUI* m_RoomEditor;

public:
	virtual void RenderUpdate() override;

public:
	EditorGUI();
	virtual ~EditorGUI();
};

