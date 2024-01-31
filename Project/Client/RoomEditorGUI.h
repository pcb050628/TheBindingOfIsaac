#pragma once
#include "GUI.h"

class Room;
class RoomEditorGUI :
    public GUI
{
private:
	Room* m_EditRoom;

public:


public:
	virtual void RenderUpdate() override;
	virtual void Activate() override;

public:
	RoomEditorGUI();
	~RoomEditorGUI();
};

