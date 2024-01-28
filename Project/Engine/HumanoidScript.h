#pragma once
#include "Script.h"

class HumanoidScript :
    public Script
{
private:
    GameObject* Head;
    GameObject* Body;
    //머리, 몸통 방향 있어야함

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    //방향 설정 함수 추가

public:
    HumanoidScript();
    ~HumanoidScript();
};

