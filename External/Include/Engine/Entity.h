#pragma once
#include <windows.h>
#include "pch.h"

#define CLONE(CLASS) virtual CLASS* Clone() { return new CLASS(*this); }
#define CLONE_DISABLE(CLASS) CLASS* Clone() { return nullptr; assert(nullptr); }\
							 CLASS(const CLASS& _OriginBuffer) = delete;

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

	virtual Entity* Clone() = 0;

protected:
	void SetDead() { m_bIsDead = true; }

public:
	Entity();
	Entity(const Entity& _origin);
	virtual ~Entity();

};
