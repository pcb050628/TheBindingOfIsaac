#pragma once

class Script;
class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:
	std::map<std::wstring, Script*> m_map;
	void Register(Script* _scrpt);

public:
	void Init();
	Script* Find(const std::wstring& _name);

	template <typename T>
	void RegisterScript()
	{
		Register(new T);
	}
};

