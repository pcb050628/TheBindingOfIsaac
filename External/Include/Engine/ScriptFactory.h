#pragma once

#define REGISTER_SCRIPT(className) \
    namespace { \
        const bool className##Registered = []() { \
            ScriptFactory::GetInst()->Register(new className); \
            return true; \
        }(); \
    }

class Script;
class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:
	std::map<std::wstring, Script*> m_map;

public:
	void Init();
	Script* Find(const std::wstring& _name);

	void Register(Script* _scrpt);
};