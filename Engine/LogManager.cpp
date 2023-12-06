#include "pch.h"
#include "LogManager.h"
#include "RenderManager.h"
#include "Time.h"





LogManager::LogManager()
	: m_LogLifeTime(1.f)
	, m_LogList()
{

}

LogManager::~LogManager()
{

}

void LogManager::Update()
{
	auto iter = m_LogList.begin();
	for (; iter != m_LogList.end();)
	{
		iter->m_AccTime += Time::GetInst()->GetDeltaTime();
		if (m_LogLifeTime < iter->m_AccTime)
		{
			iter = m_LogList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void LogManager::Render()
{
	auto iter = m_LogList.begin();
	Vec2 LT(10, 10);
	
	for (int i = 0; iter != m_LogList.end(); i++)
	{
		DirectX::XMVECTORF32 color = DirectX::Colors::White;
		switch (iter->level) // text 색상 정하기
		{
		case LOG_LEVEL::ERR:
			color = DirectX::Colors::Red;
			break;
		}

		RenderManager::GetInst()->FontRender(iter->message, LT + (Vec2(0, 1) * (float)i), color);
	}
}
