#include "pch.h"
#include "Time.h"
#include "Engine.h"

Time::Time()
	: m_Frequency()
	, m_CurCount()
	, m_PrevCount()
	, m_DeltaTime(0.f)
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

	m_DeltaTime = static_cast<float>(m_CurCount.QuadPart - m_PrevCount.QuadPart) / static_cast<float>(m_Frequency.QuadPart);

	static float count;
	count += m_DeltaTime;
	if (count > 1.f)
	{
		wchar_t szText[50] = {};
		swprintf_s(szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, static_cast<int>((1 / m_DeltaTime)));
		SetWindowText(Engine::GetInst()->GetHwnd(), szText);

		count = 0;
	}
}
