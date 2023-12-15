#include "pch.h"
#include "Resource.h"

Resource::Resource(RESOURCE_TYPE _type)
	: m_Type(_type)
{
}

Resource::~Resource()
{
}

bool Resource::Load(std::wstring _FilePath)
{
	return false;
}

bool Resource::Save()
{
	return false;
}
