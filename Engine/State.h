#pragma once
#include "Entity.h"

enum class StateID;
class State :
    public Entity
{
private:
    StateID m_ID;

public:
    virtual void Enter() = 0;
    virtual void Update() = 0;
    virtual void LateUpdate() = 0;
    virtual void Exit() = 0;

    virtual StateID GetStateID() { return m_ID; }

public:
    State();
    virtual ~State() override;
};

enum class StateID
{
    MonTrace,
    MonMoveRandomly,
    End,
};