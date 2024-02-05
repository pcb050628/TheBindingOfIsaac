#pragma once
#include "Script.h"
#include <rttr/type>

class HumanoidScript :
    public Script
{
private:
    GameObject* Head;
    GameObject* Body;
    DIRECTION   HeadDir;
    DIRECTION   BodyDir;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetHeadDir(DIRECTION _dir) { HeadDir = _dir; }
    void SetBodyDir(DIRECTION _dir) { BodyDir = _dir; }

    DIRECTION GetHeadDir() { return HeadDir; }
    DIRECTION GetBodyDir() { return BodyDir; }

public:
    HumanoidScript();
    ~HumanoidScript();
};

