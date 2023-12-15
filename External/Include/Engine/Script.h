#pragma once
#include "Component.h"

#include "components.h"

class Script :
    public Component
{
public:
    virtual void Update() {}
    virtual void LateUpdate() final {}

public:
    Script();
    ~Script();
};

