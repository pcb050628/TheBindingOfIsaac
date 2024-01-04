#pragma once
#include "Collider.h"

class CircleCollider :
    public Collider
{
private:
    float m_Radius;

public:
    void Update() override;
    void LateUpdate() override;

    void SetRadius(float _radius) { m_Radius = _radius; }
    float GetRadius() { return m_Radius; }

public:
    CircleCollider();
    ~CircleCollider() override;
};

