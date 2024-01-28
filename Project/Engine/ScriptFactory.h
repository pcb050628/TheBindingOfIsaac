#pragma once

class Script;
class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:


public:
	Script* Find(const std::wstring& _name);
};

