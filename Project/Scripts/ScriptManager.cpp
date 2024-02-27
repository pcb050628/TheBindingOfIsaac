#include "pch.h"
#include "ScriptManager.h"

#include "HumanoidScript.h"
#include "SpotLight2DMove.h"

void ScriptManager::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"HumanoidScript");
	_vec.push_back(L"SpotLight2DMove");
}

Script * ScriptManager::GetScript(const wstring& _strScriptName)
{
	if (L"HumanoidScript" == _strScriptName)
		return new HumanoidScript;
	if (L"SpotLight2DMove" == _strScriptName)
		return new SpotLight2DMove;
	return nullptr;
}

Script * ScriptManager::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::HUMANOIDSCRIPT:
		return new HumanoidScript;
		break;
	case (UINT)SCRIPT_TYPE::SPOTLIGHT2DMOVE:
		return new SpotLight2DMove;
		break;
	}
	return nullptr;
}

const wchar_t * ScriptManager::GetScriptName(Script * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::HUMANOIDSCRIPT:
		return L"HumanoidScript";
		break;

	case SCRIPT_TYPE::SPOTLIGHT2DMOVE:
		return L"SpotLight2DMove";
		break;

	}
	return nullptr;
}