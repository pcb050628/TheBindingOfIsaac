#include "pch.h"
#include "ScriptFactory.h"

#include "Script.h"

ScriptFactory::ScriptFactory()
{}

ScriptFactory::~ScriptFactory()
{
    for (auto pair : m_map)
    {
        delete pair.second;
    }
    m_map.clear();
}

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

bool ScriptFactory::IsExistance(const std::wstring& _name)
{
    auto iter = m_map.find(_name);

    if (m_map.end() == iter)
        return false;
    else
        return true;

    return false;
}

void ScriptFactory::GetAllScriptName(std::vector<std::string>& _out)
{
    std::transform(m_map.begin(), m_map.end(), std::back_inserter(_out), [](const auto& pair) { return ToString(pair.first); });
}

void ScriptFactory::GetAllScriptName(std::vector<std::wstring>& _out)
{
    std::transform(m_map.begin(), m_map.end(), std::back_inserter(_out), [](const auto& pair) { return pair.first; });
}

void ScriptFactory::Register(Script* _scrpt)
{
    if (!IsExistance(_scrpt->GetName()))
    {
        m_map.insert(make_pair(_scrpt->GetName(), _scrpt));
    }
    else
    {
        delete _scrpt;
    }
}
