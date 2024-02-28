#pragma once
#include <Engine\Script.h>

class HumanoidScript :
    public Script
{
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

    //수정 필요함
    void SaveToFile(FILE* _file) override;
    void LoadFromFile(FILE* _file) override;

    CLONE(HumanoidScript)
public:
    HumanoidScript();
    HumanoidScript(const HumanoidScript& _origin);
    ~HumanoidScript();
};