#pragma once
#include "GUI.h"

class Anim;
class AnimEditorRenderGUI;
class AnimEditorGUI :
    public GUI
{
private:
	Anim*					m_EditAnim;
	AnimEditorRenderGUI*	m_RenderGUI;

public:
	virtual void RenderUpdate() override;

public:
	void CreateNewAnim();
	void DeleteAnim();
	void SetAnimAtlas(DWORD_PTR _str);

	virtual void Activate() override;
	virtual void Deactivate() override;

public:
	AnimEditorGUI();
	virtual ~AnimEditorGUI();

	friend AnimEditorRenderGUI;
};

