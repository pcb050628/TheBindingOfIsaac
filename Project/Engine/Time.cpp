#include "pch.h"
#include "Time.h"
#include "Engine.h"

Time::Time()
	: m_Frequency()
	, m_CurCount()
	, m_PrevCount()
	, m_DeltaTime(0.f)
	, m_EngineDeltaTime(0.f)
	, m_bLockDeltaTime(false)
{
}

Time::~Time()
{

}

void Time::Init()
{
	QueryPerformanceFrequency(&m_Frequency);

	QueryPerformanceCounter(&m_CurCount);
}

void Time::Update()
{
	m_PrevCount.QuadPart = m_CurCount.QuadPart;
	QueryPerformanceCounter(&m_CurCount);

	m_EngineDeltaTime = static_cast<float>(m_CurCount.QuadPart - m_PrevCount.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	m_DeltaTime = 0.f;
	if (!m_bLockDeltaTime) m_DeltaTime = m_EngineDeltaTime;

	static float count;
	count += m_EngineDeltaTime;
	if (count > 1.f)
	{
		wchar_t szText[100] = {};
		swprintf_s(szText, 100, L"DeltaTime : %f, FPS : %d", m_EngineDeltaTime, (int)(1 / m_EngineDeltaTime));
		SetWindowText(Engine::GetInst()->GetHwnd(), (const char*)szText);

		count = 0;
	}
}
