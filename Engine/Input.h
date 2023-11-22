#pragma once
#include <windows.h>
#include "define.h"

using namespace std;

enum Key
{
	Q,W,E,R,T,Y,U,I,O,P,
	A,S,D,F,G,H,J,K,L,
	Z,X,C,V,B,N,M,

	Up,Left,Down,Right,
	MLBTN,MRBTN,
	TAB,

	END,
};

enum KeyState
{
	Pressed,
	Hold,
	Release,
	None,
};

struct KeyData
{
	Key			key;
	KeyState	state;
	bool		isPressed;

	KeyData(Key _key)
		: key(_key)
		, state(KeyState::None)
		, isPressed(false)
	{

	}
};

class Input
{
	SINGLETON(Input)
private:
	vector<KeyData> m_Keys;

public:
	void Init();
	void Update();
	void LateUpdate();

	bool GetKey(Key _key) { return m_Keys[(int)_key].isPressed; }
	bool CompareKeyState(Key _key, KeyState _state) { return m_Keys[(int)_key].state == _state; }

};

