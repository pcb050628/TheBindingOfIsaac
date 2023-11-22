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
    // 머리와 몸통 애니메이터 또는 액터를 각각 가지고 있기
    // 머리와 몸으로 이루어진 액터들은 이 클래스를 상속받을것


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

