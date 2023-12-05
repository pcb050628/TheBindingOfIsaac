#pragma once
#include "Entity.h"
#include "Room.h"

class Chapter :
    public Entity
{
private:
    vector<Room*> m_Rooms;
    Room* m_CurRoom;

public:
    virtual void Update();
    virtual void LateUpdate();
    virtual void Render();

    virtual Room* GetCurRoom() { return m_CurRoom; }

public:
    Chapter();
    virtual ~Chapter() override;

private:
    void ChangeRoom(DIRECTION _dir)
    {
        Room* room = m_CurRoom->GetRoomByDir(_dir);

        if (room != nullptr)
            m_CurRoom = room;
    }

    void AddActor(Actor* _actr, LayerType _layr)
    {
        m_CurRoom->AddActor(_actr, _layr);
    }

    friend class TaskManager;
};

