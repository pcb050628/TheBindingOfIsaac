#pragma once
#include "Script.h"

class HumanoidScript :
    public Script
{
    SCRIPT(HumanoidScript)
private:
    GameObject* Head;
    GameObject* Body;
    DIRECTION   HeadDir;
    DIRECTION   BodyDir;

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