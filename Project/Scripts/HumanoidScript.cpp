#include "pch.h"
#include "HumanoidScript.h"

HumanoidScript::HumanoidScript() : Script(L"HumanoidScript", HUMANOIDSCRIPT)
	, Head(nullptr)
	, Body(nullptr)
	, HeadDir(DIRECTION::BottomDir)
	, BodyDir(DIRECTION::BottomDir)
{}

HumanoidScript::HumanoidScript(const HumanoidScript& _origin)
	: Script(_origin)
	, Head(nullptr)
	, Body(nullptr)
	, HeadDir(DIRECTION::BottomDir)
	, BodyDir(DIRECTION::BottomDir)
{}

HumanoidScript::~HumanoidScript()
{
}

void HumanoidScript::Init()
{
	if (Head != nullptr)
	{
		Head->DisconnectWithParent();
		delete Head;
		Head = nullptr;
	}
	if (Body != nullptr)
	{
		Body->DisconnectWithParent();
		delete Body;
		Body = nullptr;
	}

	Head = new GameObject(L"Head");
	Body = new GameObject(L"Body");

	Head->AddComponent(new Transform);
	Head->AddComponent(new Collider2D);

	Body->AddComponent(new Transform);
	Body->AddComponent(new Collider2D);

	GetOwner()->AttachChild(Head);
	GetOwner()->AttachChild(Body);

	//��, �Ӹ� ��ġ ���� �ؾ���
}

void HumanoidScript::Update()
{
}