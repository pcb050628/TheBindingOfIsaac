#pragma once
#include "Component.h"

#include "GameObject.h"
#include "components.h"

class Script :
    public Component
{
private:

public:
    virtual void LateUpdate() override {}

public:
    Script();
    ~Script();
};

