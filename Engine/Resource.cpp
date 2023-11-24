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

Resource* Resource::Create()
{
	return nullptr;
}