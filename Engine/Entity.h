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
	bool m_bIsDead;

public:
	UINT GetID() { return m_ID; }

	bool GetIsDead() { return m_bIsDead; }

	void SetName(const wstring& _name) { m_Name = _name; }
	wstring GetName() { return m_Name; }

protected:
	void SetDead() { m_bIsDead = true; }

public:
	Entity();
	virtual ~Entity();

};
