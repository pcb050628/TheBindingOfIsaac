#pragma once
#include "Script.h"

class HumanoidScript :
    public Script
{
private:
    GameObject* Head;
    GameObject* Body;
    //�Ӹ�, ���� ���� �־����

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    //���� ���� �Լ� �߰�

public:
    HumanoidScript();
    ~HumanoidScript();
};

