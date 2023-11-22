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

    // ���� 15 x 9 Ÿ��
    // �ܰ� �κ��� �����ϸ� 13 x 7

    // ���߿� �����Ҷ�
    // ���̾� �� ���鼭 GetActorAboveTileY()
    // ȣ���ϸ鼭 y�� ������������ �����ϱ� / background, ui �� ����

    // ��ǥ�� 15 x 9 / directx ��ǥ�� ��ȯ
    // x ĭ�� 0.13333333f, y ĭ�� 0.22222222f

    // �⺻ �� ������ �����ؼ� �� �̵� ���� �����ŭ �̵��ϱ�
    // �� Ű�Ⱑ �־�� â ũ�Ⱑ ����ǵ� ������ ũ��� ������ ������ �� ũ�⸦ ���ؾ� ��

public:
    Room();
    virtual ~Room() override;
    
};

