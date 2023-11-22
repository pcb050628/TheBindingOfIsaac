#pragma once
#include "Entity.h"

class Actor;
class Room;
class Chapter :
    public Entity
{
private:
    vector<Room*> m_Rooms;
    Room* m_CurRoom;

public:
    virtual void Update();
    virtual void LateUpdate();

    virtual Room* GetCurRoom() { return m_CurRoom; }

public:
    Chapter();
    virtual ~Chapter() override;

};

