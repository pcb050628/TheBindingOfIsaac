#pragma once
#include "Entity.h"
#include "Layer.h"

enum class RoomType
{
    Common,
    Treasure,
    Shop,
    Boss,
};

class Room :
    public Entity
{
private:
    Layer m_Layers[(int)LayerType::END];

public:
    virtual void Update();
    virtual void LateUpdate();

    virtual Layer& GetLayer(LayerType _type) { return m_Layers[(int)_type]; }

    // 방은 15 x 9 타일
    // 외각 부분을 제외하면 13 x 7

    // 나중에 렌더할때
    // 레이어 다 돌면서 GetActorAboveTileY()
    // 호출하면서 y값 오름차순으로 렌더하기 / background, ui 등 제외

    // 좌표계 15 x 9 / directx 좌표계 변환
    // x 칸당 0.13333333f, y 칸당 0.22222222f

    // 기본 방 사이즈 설정해서 방 이동 마다 사이즈만큼 이동하기
    // 방 키기가 있어야 창 크기가 변경되도 일정한 크기로 나오기 때문에 방 크기를 정해야 함

public:
    Room();
    virtual ~Room() override;
    
};

