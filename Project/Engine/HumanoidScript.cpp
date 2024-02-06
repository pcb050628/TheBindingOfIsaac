#include "pch.h"
#include "HumanoidScript.h"

HumanoidScript::HumanoidScript() : Script(L"HumanoidScript")
	, Head(new GameObject(L"Head"))
	, Body(new GameObject(L"Body"))
	, HeadDir(DIRECTION::BottomDir)
	, BodyDir(DIRECTION::BottomDir)
{
	RegistScript<HumanoidScript>();

	Head->AddComponent(new Transform);
	Head->AddComponent(new Collider2D);

	Body->AddComponent(new Transform);
	Body->AddComponent(new Collider2D);
}

HumanoidScript::~HumanoidScript()
{
}

void HumanoidScript::Init()
{
	GetOwner()->AttachChild(Head);
	GetOwner()->AttachChild(Body);

	//��, �Ӹ� ��ġ ���� �ؾ���
}

void HumanoidScript::Update()
{
}

Script* HumanoidScript::Instance()
{
	return new HumanoidScript;
}
