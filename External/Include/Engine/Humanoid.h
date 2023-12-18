#pragma once
#include "GameObject.h"

class Humanoid :
    public GameObject
{
protected:
    enum Direction
    {
        top,
        down,
        left,
        right,
    };
private:
    // 머리
    // 몸

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

