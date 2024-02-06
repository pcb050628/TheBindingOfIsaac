#pragma once

class Script;
class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:
	std::map<std::wstring, Script*> m_map;

public:
	Script* Find(const std::wstring& _name);
	void Register(Script* _scrpt);
};


template <typename T>
constexpr void RegistScript()
{
	ScriptFactory::GetInst()->Register(new T);
}