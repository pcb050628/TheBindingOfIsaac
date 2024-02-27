#include "pch.h"
#include "Script.h"

Script::Script(const std::wstring& _name, UINT _scriptType) : Component(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_scriptType)
{
	SetName(_name);
}

Script::~Script()
{
}
