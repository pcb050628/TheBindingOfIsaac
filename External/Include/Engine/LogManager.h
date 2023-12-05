#pragma once
#include "define.h"
#include <list>

enum class LOG_LEVEL
{
	ERR,
	LOG,
};
struct Log
{
	LOG_LEVEL level;
	std::wstring message;
	float m_AccTime;
};

class LogManager
{
	SINGLETON(LogManager)
private:
	std::list<Log> m_LogList;

	float m_LogLifeTime;

public:
	void Update();
	void Render();

	void AddLog(Log _log) { m_LogList.push_front(_log); }

	void SetLogLifeTime(float _time) { m_LogLifeTime = _time; }

};

