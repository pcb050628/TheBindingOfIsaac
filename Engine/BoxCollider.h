#pragma once
#include "Collider.h"

class BoxCollider :
    public Collider
{
private:
    Vec2 m_Scale;

public:
    void Update() override;
    void LateUpdate() override;

    void SetScale(Vec2 _scale) { m_Scale = _scale; }
    Vec2 GetScale() { return m_Scale; }

public:
    BoxCollider();
    ~BoxCollider() override; 

};

