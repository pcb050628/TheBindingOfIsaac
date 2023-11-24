#pragma once
#include "Entity.h"

class State :
    public Entity
{
private:


public:
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void Exit() = 0;

public:
    State();
    virtual ~State() override;
};

