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

Script* ScriptFactory::Find(size_t _hash)
{
    if (_hash == typeid(HumanoidScript).hash_code())
        return new HumanoidScript;
    else if (_hash == typeid(SpotLight2DMove).hash_code())
        return new SpotLight2DMove;

    return nullptr;
}
