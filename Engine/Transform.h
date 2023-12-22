#pragma once
#include "Component.h"

class Transform :
    public Component
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    Vec3    m_vLocalDir[(UINT)DIR_TYPE::END];
    Vec3    m_vWorldDir[(UINT)DIR_TYPE::END];

    Matrix  m_matWorld;
    bool    m_bAbsolute;

public:
    void Update() override;
    void LateUpdate() override;
    void UpdateData() override;

public:
    Vec3 GetRelativePos() { return m_vRelativePos; }
    Vec3 GetRelativeScale() { return m_vRelativeScale; }
    Vec3 GetRelativeRotation() { return m_vRelativeRot; }
    Vec3 GetLocalDir(DIR_TYPE _type) { return m_vLocalDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) { return m_vWorldDir[(UINT)_type]; }

    Matrix GetWorldMat() { return m_matWorld; }

    void SetRelativePos(Vec3 _pos) { m_vRelativePos = _pos; }
    void SetRelativeScale(Vec3 _scale) { m_vRelativeScale = _scale; }
    void SetRelativeRot(Vec3 _rotate) { m_vRelativeRot = _rotate; }

public:
    Transform();
    ~Transform() override;
};

