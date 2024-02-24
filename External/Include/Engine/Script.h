#pragma once
#include "Component.h"

#include "Input.h"
#include "Time.h"

#include "GameObject.h"
#include "components.h"

#include "ScriptFactory.h"

#define GET_OTHER_COMPONENT(Type) Type* Get##Type() { return GetOwner()->Get##Type(); }
#define SCRIPT(SCRIPTNAME) public: class Script* Instance() override { return new SCRIPTNAME; }

class Script :
    public Component
{
public:
    virtual void Enter() {}
    virtual void Init() {}
    virtual void Update() override {}
    virtual void LateUpdate() final {}
    virtual void Exit() {}

    virtual Script* Instance() = 0;

    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRenderer);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Animator2D);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Light2D);


public:
    Script(const std::wstring& _name);
    ~Script();
};

