#pragma once
#include "GUI.h"

class AnimEditorGUI;
class EditorGUI :
    public GUI
{
private:
	AnimEditorGUI* m_AnimEditor;

public:
	virtual void RenderUpdate() override;

public:
	EditorGUI();
	virtual ~EditorGUI();
};

