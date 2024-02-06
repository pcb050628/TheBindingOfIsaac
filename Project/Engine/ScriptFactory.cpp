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

void ScriptFactory::Init()
{
}

Script* ScriptFactory::Find(const std::wstring& _name)
{
    auto iter = m_map.find(_name);

    if (m_map.end() == iter)
        return nullptr;
    else
        return iter->second->Instance();

    return nullptr;
}

void ScriptFactory::Register(Script* _scrpt)
{
    if (Find(_scrpt->GetName()) == nullptr)
    {
        m_map.insert(make_pair(_scrpt->GetName(), _scrpt));
    }
    else
    {
        int a = 0;
    }
}
