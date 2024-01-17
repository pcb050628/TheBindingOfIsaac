#pragma once
#include "Chapter.h"

#include "EditRoom.h"

class EditChapter :
    public Chapter
{
private:
    EditRoom* m_CurRoom;

public:
    virtual void Update();
    virtual void LateUpdate();

    virtual Room* GetCurRoom() { return (Room*)m_CurRoom; }

    void DetachGameObject(GameObject* _obj);

private:
    void AddObject(GameObject* _actr, LAYER_TYPE _layr, bool _bMove)
    {
        m_CurRoom->AddObject(_actr, _layr, _bMove);
    }

public:
    EditChapter();
    virtual ~EditChapter() override;
};

