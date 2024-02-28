#pragma once
#include "Resource.h"
#include "Layer.h"

enum class ROOM_STATE
{
    PLAY, // play or continue
    STOP, // STOP like pause
    EDIT, // editMode
    NONE, // none
};

enum class ROOM_TYPE
{
    Common,
    Treasure,
    Shop,
    Boss,
};

struct RoomInfo
{
    UINT            RoomNumber;
    ROOM_TYPE       RoomType = ROOM_TYPE::Common;
    UINT            ChapterLevel = 0;
    bool            IsCompleted = false;
    //prefab info
    std::wstring    TileObjectInfo[15][9] = {};
    LAYER_TYPE      TileLayerInfo[15][9] = {};
};

class Room :
    public Resource
{
private:
    RoomInfo    m_Info;
    ROOM_STATE  m_State;
    Layer*      m_Layers[(int)LAYER_TYPE::END];
    GameObject* m_MainCam;

    Room*       Left;
    Room*       Right;
    Room*       Top;
    Room*       Bottom;

    bool        m_bEditMode;

public:
    virtual bool Load(const std::wstring& _strFileName, bool _isFullPath) override;
    virtual bool Save() override;

    virtual void Clear();

    virtual void Enter();
    virtual void Begin();
    virtual void Update();
    virtual void LateUpdate();
    virtual void Stop();
    virtual void Exit();

    virtual ROOM_TYPE GetRoomType() { return m_Info.RoomType; }

    virtual Layer* GetLayer(int _type) { return m_Layers[_type]; }
    virtual Layer* GetLayer(LAYER_TYPE _type) { return GetLayer((int)_type); }

    virtual bool IsComplete() { return m_Info.IsCompleted; }

    virtual Room* GetRoomByDir(DIRECTION _dir)
    {
        switch (_dir)
        {
        case LeftDir:
            return Left;
        case RightDir:
            return Right;
        case TopDir:
            return Top;
            break;
        case BottomDir:
            return Bottom;
        }

        return nullptr;
    }

    //x row / y col
    Vec3 GetPosByTile(UINT _x, UINT _y);
    Vec3 GetPosByTile(Vec2 _tile);
    Vec2 GetTileByPos(Vec3 _pos);

    void AddObject(GameObject* _obj, LAYER_TYPE _layr, bool _bMove);
    void AddObjectByTile(GameObject* _obj, LAYER_TYPE _layr, Vec2 _tilePos, bool _bMove);
    void DetachGameObject(GameObject* _obj);

    GameObject* GetMainCam() { return m_MainCam; }
    int SetMainCam(GameObject* _cam);

    GameObject* FindObject(const std::wstring& _strName);
    virtual void GetAllObject(std::vector<GameObject*>& _out);

    void ChangeState(ROOM_STATE _state);

    CLONE(Room)
public:
    Room();
    Room(const Room& _origin);
    virtual ~Room() override;

    friend class Chapter;
    friend class RoomEditorGUI;
    friend class RoomEditorRenderGUI;
};

