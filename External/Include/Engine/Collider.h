#pragma once
#include "Component.h"


enum class ColliderType
{
    Circle,
    Box,
};

class Collider :
    public Component
{
private:
    ColliderType m_Type;
    Vec2 m_PosOffset;
    
public:
    virtual void Update() override;
    virtual void LateUpdate() override;

    virtual ColliderType GetColliderType() { return m_Type; }

    virtual void SetPosOffset(Vec2 _offset) { m_PosOffset = _offset; }
    virtual Vec2 GetPosOffset() { return m_PosOffset; }

    virtual Vec2 GetFinalPos();

    virtual void BeginOverlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol) = 0;
    virtual void Overlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol) = 0;
    virtual void EndOverlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol) = 0;

public:
    Collider(ColliderType _type);
    virtual ~Collider() = 0;
};