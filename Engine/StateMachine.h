#pragma once
#include "Component.h"
#include "State.h"

class StateMachine :
    public Component
{
    typedef Component Super;
private:
    std::map<StateID, State*>          m_States;
    State*                          m_CurState;

    std::map<std::wstring, void*>   m_BlackBoard;

public:
    virtual void Update() override;
    virtual void LateUpdate() override;

    bool AddState(State* _state)
    {
        auto iter = m_States.find(_state->GetStateID());
        if (iter != m_States.end())
            return false;

        m_States.insert(std::make_pair(_state->GetStateID(), _state));
        return true;
    }

    bool ChangeState(StateID _id)
    {
        auto iter = m_States.find(_id);
        if (iter != m_States.end())
        {
            m_CurState->Exit();
            m_CurState = iter->second;
            m_CurState->Enter();
            return true;
        }
        return false;
    }


public:
    StateMachine();
    ~StateMachine() override;
};

