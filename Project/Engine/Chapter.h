#pragma once
#include "Entity.h"
#include "Room.h"

class Chapter :
    public Entity
{
private:
    vector<Room*> m_Rooms;
    Room* m_CurRoom;

    bool m_bEditMode;

public:
    virtual void Update();
    virtual void LateUpdate();

    virtual Room* GetCurRoom() { return m_CurRoom; }

    void DetachGameObject(GameObject* _obj);

    void GenerateRooms(CHAPTER_LEVEL _level);

    void SetEditMode(bool _bValue);

private:
    //Room 의 이동시 트랜지션이 있어야 할듯
    //현재 Room이 _dir 의 반대 방향으로 밀리면서 _dir 방향으로 바뀔 Room 이 밀고 들어오는 식
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

