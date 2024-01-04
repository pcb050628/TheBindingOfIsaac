#pragma once
#include "Actor.h"

class Humanoid :
    public Actor
{
    typedef Actor Super;
protected:
    enum Direction
    {
        top,
        down,
        left,
        right,
    };

private:
    // �Ӹ��� ���� �ִϸ����� �Ǵ� ���͸� ���� ������ �ֱ�
    // �Ӹ��� ������ �̷���� ���͵��� �� Ŭ������ ��ӹ�����


public:
    virtual void Update() override;
    virtual void LateUpdate() override;

    //virtual void SetHeadDir(Direction dir);
    //virtual void SetBodyDir(Direction dir);
    //virtual Direction GetHeadDir();
    //virtual Direction GetBodyDir();

public:
    Humanoid();
    virtual ~Humanoid() override;

};

