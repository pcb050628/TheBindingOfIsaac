#include "pch.h"
#include "StateMachine.h"

StateMachine::StateMachine() : Super(ComponentType::STATEMACHINE)
{
}

StateMachine::~StateMachine()
{
}

void StateMachine::Update()
{
	if (m_CurState != nullptr)
		m_CurState->Update();
}

void StateMachine::LateUpdate()
{
	if (m_CurState != nullptr)
		m_CurState->LateUpdate();
}
