#include "pch.h"
#include "Script.h"

Script::Script(const std::wstring& _name) : Component(COMPONENT_TYPE::SCRIPT)
{
	SetName(_name);
}

Script::~Script()
{
}
