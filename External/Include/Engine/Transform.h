#pragma once
#include "Component.h"

class Transform :
    public Component
{
private:
    Vec3 m_vPosition;
    Vec3 m_vScale;
    Vec3 m_vRotation;

    Vec3 m_vDir[(UINT)DIR_TYPE::END];

    Matrix m_matWorld;

public:
    void Update() override;
    void LateUpdate() override;
    void UpdateData() override;

public:
    Vec3 GetPos() { return m_vPosition; }
    Vec3 GetScale() { return m_vScale; }
    Vec3 GetRotation() { return m_vRotation; }

    void SetPos(Vec3 _pos) { m_vPosition = _pos; }
    void SetScale(Vec3 _scale) { m_vScale = _scale; }
    void SetRotation(Vec3 _rotate) { m_vRotation = _rotate; }

public:
    Transform();
    ~Transform() override;
};

