#pragma once
#include <Windows.h>
#include "define.h"

#define DT Time::GetInst()->GetDeltaTime();

class Time
{
	SINGLETON(Time)
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;
	float			m_DeltaTime;
	float			m_EngineDeltaTime;

	bool			m_bLockDeltaTime;

public:
	void Init();
	void Update();

	float GetDeltaTime() { return m_DeltaTime; }

	void LockDeltaTime(bool _value) { m_bLockDeltaTime = _value; }
};

