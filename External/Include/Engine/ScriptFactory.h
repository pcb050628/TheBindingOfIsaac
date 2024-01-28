#pragma once

class Script;
class ScriptFactory
{
	SINGLETON(ScriptFactory)
private:


public:
	Script* Find(size_t _hash);
};

