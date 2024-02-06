#pragma once
#include "GUI.h"

class Room;
class RoomEditorRenderGUI;
class RoomEditorGUI :
    public GUI
{
private:
	RoomEditorRenderGUI*	m_RenderGUI;

	Room*					m_EditRoom;
	std::wstring			m_SelectObject;
	LAYER_TYPE				m_SelectLayer;

public:
	void ActivateRenderGUI();

	void SelectObject(DWORD_PTR _str);

public:
	void RenderUpdate() override;
	void Activate() override;

public:
	RoomEditorGUI();
	~RoomEditorGUI();

	friend class RoomEditorRenderGUI;
};
