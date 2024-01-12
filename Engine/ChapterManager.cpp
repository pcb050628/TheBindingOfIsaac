#include "pch.h"
#include "ChapterManager.h"
#include "TaskManager.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "components.h"

ChapterManager::ChapterManager()
	: m_CurChapter(nullptr)
	, m_Chapters{}
{

}

ChapterManager::~ChapterManager()
{
	for (Chapter* chptr : m_Chapters)
	{
		delete chptr;
	}
}

void ChapterManager::Init()
{
	for (int i = 0; i < 4; i++)
	{
		m_Chapters[i] = new Chapter;
	}

	m_CurChapter = m_Chapters[0];

	// Anim Test obj
	GameObject* gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new MeshRenderer);
	gobj->AddComponent(new Animator2D);
		
	gobj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"default_Material"));
	gobj->GetMeshRenderer()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"RectMesh"));

	gobj->GetAnimator2D()->CreateAnim(L"test_anim", ResourceManager::GetInst()->Find<Texture>(L"Rocks"), Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 0.f), Vec2(40.f, 40.f), 6, 1);
	gobj->GetAnimator2D()->Play(L"test_anim", true);

	gobj->SetName(L"test_player");
	AddGameObject(gobj, LAYER_TYPE::Player);

	// UI Test obj
	gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new MeshRenderer);

	gobj->GetMeshRenderer()->SetMaterial(ResourceManager::GetInst()->Find<Material>(L"ui_Material")); 
	gobj->GetMeshRenderer()->SetMesh(ResourceManager::GetInst()->Find<Mesh>(L"RectMesh")); 
	gobj->GetMeshRenderer()->GetMaterial()->SetTexture(ResourceManager::GetInst()->Find<Texture>(L"Rocks"), TEX_0);

	gobj->SetName(L"test_ui");
	AddGameObject(gobj, LAYER_TYPE::UI);

	// Main Cam
	gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new Camera);

	gobj->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));

	gobj->GetCamera()->SetCameraPriority(0);
	gobj->GetCamera()->LayerCheckAll(true);
	gobj->GetCamera()->LayerCheck(LAYER_TYPE::UI, false);

	gobj->SetName(L"MAIN_CAM");
	AddGameObject(gobj, LAYER_TYPE::Camera);

	// UI Cam
	//gobj = new GameObject;
	//gobj->AddComponent(new Transform);
	//gobj->AddComponent(new Camera);
	//
	//gobj->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, -10.f));
	//
	//gobj->GetCamera()->SetCameraPriority(1);
	//gobj->GetCamera()->LayerCheck(LAYER_TYPE::UI, true);
	//
	//gobj->SetName(L"UI_CAM");
	//AddGameObject(gobj, LAYER_TYPE::Camera);


	gobj = new GameObject;
	gobj->AddComponent(new Transform);
	gobj->AddComponent(new Light2D);

	gobj->GetTransform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));

	gobj->GetLight2D()->SetLigthType(LIGHT_TYPE::DIRECTIONAL);
	gobj->GetLight2D()->SetAmbient(Vec4(1.f));

	AddGameObject(gobj, LAYER_TYPE::Light);
}

void ChapterManager::Update()
{
	m_CurChapter->Update();
	m_CurChapter->LateUpdate();
}

void ChapterManager::Render()
{
	m_CurChapter->Render();
}

void ChapterManager::DetachGameObject(GameObject* _obj)
{
	m_CurChapter->DetachGameObject(_obj);
}

void ChapterManager::RegisterObj(GameObject* _obj, LAYER_TYPE _layerType)
{
	m_CurChapter->GetCurRoom()->GetLayer(_layerType)->RegisterObject(_obj);
}

