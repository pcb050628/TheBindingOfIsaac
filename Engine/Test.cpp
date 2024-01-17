#include "pch.h"
#include "Test.h"

#include "ResourceManager.h"
#include "Texture.h"

#include "components.h"

#include "GraphicsShader.h"
#include "Material.h"

Test::Test()
	: m_Gobjs()
{

}

Test::~Test()
{

}

void Test::Init()
{
	GameObject* obj = new GameObject;

	obj->AddComponent(new Transform);
	obj->AddComponent(new MeshRenderer);

	MeshRenderer* mr = obj->GetMeshRenderer();
	mr->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"RectMesh"));
	//mr->SetShader(ResourceManager::GetInst()->Find<GraphicsShader>(L"test_Shader"));

	mr->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"test_Material"));
	//mr->GetMaterial()->SetShader(ResourceManager::GetInst()->Find<GraphicsShader>(L"test_Shader"));

	//mr->GetMaterial()->Save();

	obj->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	obj->GetTransform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	m_Gobjs.push_back(obj);

	GameObject* cam = new GameObject;
	
	cam->AddComponent(new Transform);
	cam->AddComponent(new Camera);
	
	cam->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	cam->GetTransform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	
	m_Gobjs.push_back(cam);
}

void Test::Update()
{
	for (GameObject* gobj : m_Gobjs)
		gobj->Update();
}

void Test::LateUpdate()
{
	for (GameObject* gobj : m_Gobjs)
		gobj->LateUpdate();
}

void Test::Render()
{
	for (GameObject* gobj : m_Gobjs)
		gobj->Render();
}
