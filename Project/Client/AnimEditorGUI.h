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

	Vec2					m_AllSliceSize;
	Vec2					m_AllBackground;
	float					m_AllDuration;

public:
	virtual void RenderUpdate() override;

public:
	void SetAll();

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

