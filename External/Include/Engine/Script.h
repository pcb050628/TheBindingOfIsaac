#pragma once
#include "Component.h"

#include "Input.h"
#include "Time.h"

#include "GameObject.h"
#include "components.h"

#define GET_OTHER_COMPONENT(Type) Type* Get##Type() { return GetOwner()->Get##Type(); }

class Script :
    public Component
{
private:
    const UINT m_ScriptType;

public:
    virtual void Enter() {}
    virtual void Init() {}
    virtual void Update() override {}
    virtual void LateUpdate() final {}
    virtual void Exit() {}

    void BeginOverlap(Collider2D* _other) {}
    void Overlap(Collider2D* _other) {}
    void EndOverlap(Collider2D* _other) {}

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRenderer);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Light2D);

    UINT GetScriptType() { return m_ScriptType; }

    virtual void SaveToFile(FILE* _file) = 0;
    virtual void LoadFromFile(FILE* _file) = 0;

    virtual Script* Clone() = 0;
public:
    Script(const std::wstring& _name, UINT _scriptType);
    ~Script();
};

