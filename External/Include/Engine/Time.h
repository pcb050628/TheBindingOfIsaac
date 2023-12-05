#pragma once
#include <Windows.h>
#include "define.h"

class Time
{
	SINGLETON(Time)
private:
	LARGE_INTEGER	m_Frequency;
	LARGE_INTEGER	m_PrevCount;
	LARGE_INTEGER	m_CurCount;
	float			m_DeltaTime;

public:
	void Init();
	void Update();

	float GetDeltaTime() { return m_DeltaTime; }

};

