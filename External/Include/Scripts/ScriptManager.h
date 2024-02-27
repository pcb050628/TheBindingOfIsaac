#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	HUMANOIDSCRIPT,
	SPOTLIGHT2DMOVE,
	END,
};

using namespace std;

class Script;

class ScriptManager
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static Script * GetScript(const wstring& _strScriptName);
	static Script * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(Script * _pScript);
};
