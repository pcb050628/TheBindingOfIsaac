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

    void DetachGameObject(GameObject* _obj);

    void GenerateRooms(CHAPTERLEVEL _level);

private:
    void ChangeRoom(DIRECTION _dir)
    {
        Room* room = m_CurRoom->GetRoomByDir(_dir);

        if (room != nullptr)
            m_CurRoom = room;
    }

    void AddObject(GameObject* _actr, LAYER_TYPE _layr, bool _bMove)
    {
        m_CurRoom->AddObject(_actr, _layr, _bMove);
    }

public:
    Chapter();
    virtual ~Chapter() override;

    friend class TaskManager;
};

