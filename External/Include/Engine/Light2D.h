#pragma once
#include "Component.h"

class Light2D :
    public Component
{
private:
    tLightInfo      m_Info;

public:
    void LateUpdate() override;

    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightColor(Vec4 _color) { m_Info.vColor = _color; }
    void SetSpecular(Vec4 _spec) { m_Info.vSpecular = _spec; }
    void SetAmbient(Vec4 _ambient) { m_Info.vAmbient = _ambient; }
    void SetLigthType(LIGHT_TYPE _type) { m_Info.LightType = (int)_type; }
    void SetRadius(float _radius) { m_Info.fRadius = _radius; }
    void SetAngle(float _angle) { m_Info.fAngle = _angle; }

    Vec4 GetColor() { return m_Info.vColor; }
    Vec4 GetSpecular() { return m_Info.vSpecular; }
    Vec4 GetAmbient() { return m_Info.vAmbient; }
    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_Info.LightType; }
    float GetRadius() { return m_Info.fRadius; }
    float GetAngle() { return m_Info.fAngle; }

public:
    Light2D();
    ~Light2D();

    friend class Light2DGUI;
};

