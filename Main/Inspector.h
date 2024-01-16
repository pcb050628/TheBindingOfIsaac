#pragma once
#include "GUI.h"

class GameObject;
class Resouce;

class TransformGUI;
class MeshRendererGUI;
class CameraGUI;
class Animator2DGUI;
class Collider2DGUI;
class Light2DGUI;

class Inspector :
    public GUI
{
private:
    GameObject*         m_TargetObject;
    Resouce*            m_TargetResource;

    TransformGUI*       m_TransformGUI;
    MeshRendererGUI*    m_MeshRendererGUI;
    CameraGUI*          m_CameraGUI;
    Animator2DGUI*      m_Animator2DGUI;           
    Collider2DGUI*      m_Collider2DGUI;
    Light2DGUI*         m_Light2DGUI;

public:
    void RenderUpdate() override;

    void SetTargetObject(GameObject* _object);
    void SetTargetResource(Resouce* _resouce);

public:
    Inspector();
    ~Inspector();
};

