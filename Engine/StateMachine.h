#pragma once
#include "Component.h"
#include "State.h"

class StateMachine :
    public Component
{
private:
    std::map<UINT, State*>  m_States;
    State*                          m_CurState;

    std::map<std::wstring, void*>   m_BlackBoard;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;

public:
    StateMachine();
    ~StateMachine() override;
};

