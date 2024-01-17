#pragma once
#include "Component.h"

enum class COLLIDER2D_TYPE
{
    BOX,
    CIRCLE,
};

class Collider2D :
    public Component
{
private:
    Vec3    m_vOffsetPos;
    Vec3    m_vOffsetScale;
    bool    m_bAbsolute;

    Matrix          m_matColWorld;
    COLLIDER2D_TYPE m_Type;

public:
    void LateUpdate() override;
    
    void BeginOverlap(Collider2D* _other) {}
    void Overlap(Collider2D* _other) {}
    void EndOverlap(Collider2D* _other) {}

public:
    void SetOffsetPos(Vec2 _pos) { m_vOffsetPos = Vec3(_pos.x, _pos.y, 0.f); }
    void SetOffsetPos(float _x, float _y) { m_vOffsetPos = Vec3(_x, _y, 0.f); }
    void SetOffsetScale(Vec2 _scale) { m_vOffsetScale = Vec3(_scale.x, _scale.y, 1.f); }
    void SetOffsetScale(float _x, float _y) { m_vOffsetScale = Vec3(_x, _y, 1.f); }
    void SetAbsolute(bool _value) { m_bAbsolute = _value; }
    void SetType(COLLIDER2D_TYPE _type) { m_Type = _type; }

    Vec2 GetOffsetPos() { return Vec2(m_vOffsetPos.x, m_vOffsetPos.y); }
    Vec2 GetOffsetScale() { return Vec2(m_vOffsetScale.x, m_vOffsetScale.y); }
    bool IsAbsolute() { return m_bAbsolute; }
    COLLIDER2D_TYPE GetCollider2DType() { return m_Type; }

    const Matrix& GetColliderWorldMat() { return m_matColWorld; }

public:
    Collider2D();
    ~Collider2D();
};

