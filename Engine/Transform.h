#pragma once
#include "Component.h"

class Transform :
    public Component
{
private:
    Vec3 m_vPosition;
    Vec3 m_vScale;
    Vec3 m_vRotation;

    Vec3 m_vDir[3];

    Matrix m_matWorld;

public:
    void Update() override;
    void LateUpdate() override;
    void UpdateData() override;

public:
    Transform();
    ~Transform() override;
};

