#pragma once

#define REGISTER_SCRIPT(className) \
        const bool CallRegisteredBoolValue = []() \
        { \
            ScriptFactory::GetInst()->Register(new className); \
            return true; \
        }(); \

#include "Script.h"

class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:
	std::map<std::wstring, Script*> m_map;

public:
	void Init();
    
    //this function is instancing script
    //if you want to check whether the script exists, use IsExistance Function
	Script* Find(const std::wstring& _name);
    bool IsExistance(const std::wstring& _name);

    void GetAllScriptName(std::vector<std::string>& _out);
    void GetAllScriptName(std::vector<std::wstring>& _out);

	void Register(Script* _scrpt);

};