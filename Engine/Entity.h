#pragma once
#include <windows.h>
#include "pch.h"

using namespace std;

class Entity
{
private:
	static UINT m_NextID;

private:
	UINT m_ID;
	wstring m_Name;
	bool mb_IsDead;

public:
	UINT GetID() { return m_ID; }

	bool GetIsDead() { return mb_IsDead; }

	void SetName(const wstring& _name) { m_Name = _name; }
	wstring GetName() { return m_Name; }

public:
	Entity();
	virtual ~Entity();

};
