#pragma once
#include <windows.h>
#include "define.h"

using namespace std;

#define KEY_HOLD(key) Input::GetInst()->CompareKeyState(Key::##key, KeyState::Hold)
#define KEY_PRESSED(key) Input::GetInst()->CompareKeyState(Key::##key, KeyState::Pressed)
#define KEY_RELEASE(key) Input::GetInst()->CompareKeyState(Key::##key, KeyState::Release)

enum class Key
{
	Q,W,E,R,T,Y,U,I,O,P,
	A,S,D,F,G,H,J,K,L,
	Z,X,C,V,B,N,M,

	UP,LEFT,DOWN,RIGHT,
	MLBTN,MRBTN,
	TAB,

	END,
};

enum class KeyState
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

