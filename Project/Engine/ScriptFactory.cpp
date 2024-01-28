#include "pch.h"
#include "ScriptFactory.h"

#include "Script.h"
//typeid(Script).name();

#include "HumanoidScript.h"
#include "SpotLight2DMove.h"

ScriptFactory::ScriptFactory()
{}

ScriptFactory::~ScriptFactory()
{}

Script* ScriptFactory::Find(const std::wstring& _name)
{
    if (_name == L"HumanoidScript")
        return new HumanoidScript;
    else if (_name == L"SpotLight2DMove")
        return new SpotLight2DMove;

    return nullptr;
}
