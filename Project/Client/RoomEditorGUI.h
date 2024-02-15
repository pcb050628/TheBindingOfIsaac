#pragma once
#include "GUI.h"

class Room;
class RoomEditorRenderGUI;
class DockSpaceGUI;
class RoomEditorGUI :
    public GUI
{
private:
	DockSpaceGUI*			m_DockSpace;

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
	void Deactivate() override;

public:
	RoomEditorGUI();
	~RoomEditorGUI();

	friend class RoomEditorRenderGUI;
};

