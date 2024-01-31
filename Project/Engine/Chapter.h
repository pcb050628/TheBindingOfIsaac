#pragma once
#include "Entity.h"
#include "Room.h"

class Chapter :
    public Entity
{
private:
    vector<Room*> m_Rooms;
    Room* m_CurRoom;
    //ui cam

    bool m_bIsTransitioning;

    bool m_bEditMode;

public:
    virtual void Update();
    virtual void LateUpdate();

    virtual Room* GetCurRoom() { return m_CurRoom; }
    virtual Room* GetRoom(int _num) { Room* room = nullptr; (_num < (int)m_Rooms.size()) ? room = m_Rooms[_num] : room = nullptr; return room; }

    void DetachGameObject(GameObject* _obj);

    void GenerateRooms(CHAPTER_LEVEL _level);

    void SetEditMode(bool _bValue);

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

    void AddRoom(Room* _room);

public:
    Chapter();
    virtual ~Chapter() override;

    friend class TaskManager;
};

