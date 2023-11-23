#include "pch.h"
#include "Resource.h"

Resource::Resource(ResourceType _type)
{
}

Resource::~Resource()
{
}

bool Resource::Load(std::wstring _FilePath)
{
	return false;
}

Resource* Resource::Create(std::wstring _ResourcePath)
{
	return nullptr;
}

bool Resource::Save()
{
	return false;
}